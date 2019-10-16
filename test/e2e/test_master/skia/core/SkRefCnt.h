/*
 * Copyright 2006 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef SkRefCnt_DEFINED
#  define SkRefCnt_DEFINED
#  include "include/core/SkTypes.h"
#  include <atomic>
#  include <cstddef>
#  include <iosfwd>
#  include <memory>
#  include <type_traits>
#  include <utility>
/** \class SkRefCntBase

    SkRefCntBase is the base class for objects that may be shared by multiple
    objects. When an existing owner wants to share a reference, it calls ref().
    When an owner wants to release its reference, it calls unref(). When the
    shared object's reference count goes to zero as the result of an unref()
    call, its (virtual) destructor is called. It is an error for the
    destructor to be called explicitly (or via the object going out of scope on
    the stack or calling delete) if getRefCnt() > 1.
*/
class SK_API SkRefCntBase
{
public:
    /** Default construct, initializing the reference count to 1.
    */
  SkRefCntBase()
    : fRefCnt(1)
  {
  }
    /** Destruct, asserting that the reference count is 1.
    */
  virtual ~SkRefCntBase()
  {
#  ifdef SK_DEBUG
    SkASSERTF(this->getRefCnt() == 1, "fRefCnt was %d", this->getRefCnt());
    fRefCnt.store(0, std::memory_order_relaxed);
#  endif
  }
    /** May return true if the caller is the only owner.
     *  Ensures that all previous owner's actions are complete.
     */
  bool unique() const
  {
    if (1 == fRefCnt.load(std::memory_order_acquire))
    {
      return true;
    }
    return false;
  }
    /** Increment the reference count. Must be balanced by a call to unref().
    */
  void ref() const
  {
    SkASSERT(this->getRefCnt() > 0);
    (void) fRefCnt.fetch_add(1, std::memory_order_relaxed);
  }
    /** Decrement the reference count. If the reference count is 1 before the
        decrement, then delete the object. Note that if this is the case, then
        the object needs to have been allocated via new, and not on the stack.
    */
  void unref() const
  {
    SkASSERT(this->getRefCnt() > 0);
    if (1 == fRefCnt.fetch_add(-1, std::memory_order_acq_rel))
    {
      this->internal_dispose();
    }
  }
#  ifdef SK_DEBUG
private:
    /** Return the reference count. Use only for debugging. */
  int32_t getRefCnt() const
  {
    return fRefCnt.load(std::memory_order_relaxed);
  }
#  endif
    /**
     *  Called when the ref count goes to 0.
     */
  virtual void internal_dispose() const
  {
#  ifdef SK_DEBUG
    SkASSERT(0 == this->getRefCnt());
    fRefCnt.store(1, std::memory_order_relaxed);
#  endif
    delete this;
  }
  friend class SkWeakRefCnt;
  mutable std::atomic<int32_t> fRefCnt;
  SkRefCntBase(SkRefCntBase&&) = delete;
  SkRefCntBase(const SkRefCntBase&) = delete;
  SkRefCntBase& operator=(SkRefCntBase&&);
  SkRefCntBase& operator=(const SkRefCntBase&);
};
#  ifdef SK_REF_CNT_MIXIN_INCLUDE
#    include SK_REF_CNT_MIXIN_INCLUDE
#  else 
class SK_API SkRefCnt : public SkRefCntBase
{
#    if  defined(SK_BUILD_FOR_GOOGLE3)
public:
  void deref() const
  {
    this->unref();
  }
#    endif
};
#  endif
///////////////////////////////////////////////////////////////////////////////

/** Call obj->ref() and return obj. The obj must not be nullptr.
 */
template <typename T>
static T* SkRef(T* obj)
{
  SkASSERT(obj);
  obj->ref();
  return obj;
}
/** Check if the argument is non-null, and if so, call obj->ref() and return obj.
 */
template <typename T>
static T* SkSafeRef(T* obj)
{
  if (obj)
  {
    obj->ref();
  }
  return obj;
}
/** Check if the argument is non-null, and if so, call obj->unref()
 */
template <typename T>
static void SkSafeUnref(T* obj)
{
  if (obj)
  {
    obj->unref();
  }
}
///////////////////////////////////////////////////////////////////////////////
template <typename Derived>
class SkNVRefCnt
{
public:
  SkNVRefCnt()
    : fRefCnt(1)
  {
  }
  ~SkNVRefCnt()
  {
#  ifdef SK_DEBUG
    int rc = fRefCnt.load(std::memory_order_relaxed);
    SkASSERTF(rc == 1, "NVRefCnt was %d", rc);
#  endif
  }
  bool unique() const
  {
    return 1 == fRefCnt.load(std::memory_order_acquire);
  }
  void ref() const
  {
    (void) fRefCnt.fetch_add(1, std::memory_order_relaxed);
  }
  void unref() const
  {
    if (1 == fRefCnt.fetch_add(-1, std::memory_order_acq_rel))
    {
      SkDEBUGCODE(fRefCnt.store(1, std::memory_order_relaxed));
      delete (const Derived*) this;
    }
  }
  void deref() const
  {
    this->unref();
  }
private:
  mutable std::atomic<int32_t> fRefCnt;
  SkNVRefCnt(SkNVRefCnt&&) = delete;
  SkNVRefCnt(const SkNVRefCnt&) = delete;
  SkNVRefCnt& operator=(SkNVRefCnt&&);
  SkNVRefCnt& operator=(const SkNVRefCnt&);
};
///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 *  Shared pointer class to wrap classes that support a ref()/unref() interface.
 *
 *  This can be used for classes inheriting from SkRefCnt, but it also works for other
 *  classes that match the interface, but have different internal choices: e.g. the hosted class
 *  may have its ref/unref be thread-safe, but that is not assumed/imposed by sk_sp.
 */
template <typename T>
class sk_sp
{
  using element_type = T;
public:
  sk_sp()
    : fPtr(nullptr)
  {
  }
  sk_sp(std::nullptr_t)
    : fPtr(nullptr)
  {
  }
    /**
     *  Shares the underlying object by calling ref(), so that both the argument and the newly
     *  created sk_sp both have a reference to it.
     */
  sk_sp(const sk_sp<T>& that)
    : fPtr(SkSafeRef(that.get()))
  {
  }
  template <typename U, typename  = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
  sk_sp(const sk_sp<U>& that)
    : fPtr(SkSafeRef(that.get()))
  {
  }
    /**
     *  Move the underlying object from the argument to the newly created sk_sp. Afterwards only
     *  the new sk_sp will have a reference to the object, and the argument will point to null.
     *  No call to ref() or unref() will be made.
     */
  sk_sp(sk_sp<T>&& that)
    : fPtr(that.release())
  {
  }
  template <typename U, typename  = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
  sk_sp(sk_sp<U>&& that)
    : fPtr(that.release())
  {
  }
    /**
     *  Adopt the bare pointer into the newly created sk_sp.
     *  No call to ref() or unref() will be made.
     */
  explicit sk_sp(T* obj)
    : fPtr(obj)
  {
  }
    /**
     *  Calls unref() on the underlying object pointer.
     */
  ~sk_sp()
  {
    SkSafeUnref(fPtr);
    SkDEBUGCODE(fPtr = nullptr);
  }
  sk_sp<T>& operator=(std::nullptr_t)
  {
    this->reset();
    return *this;
  }
    /**
     *  Shares the underlying object referenced by the argument by calling ref() on it. If this
     *  sk_sp previously had a reference to an object (i.e. not null) it will call unref() on that
     *  object.
     */
  sk_sp<T>& operator=(const sk_sp<T>& that)
  {
    if (this != &that)
    {
      this->reset(SkSafeRef(that.get()));
    }
    return *this;
  }
  template <typename U, typename  = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
  sk_sp<T>& operator=(const sk_sp<U>& that)
  {
    this->reset(SkSafeRef(that.get()));
    return *this;
  }
    /**
     *  Move the underlying object from the argument to the sk_sp. If the sk_sp previously held
     *  a reference to another object, unref() will be called on that object. No call to ref()
     *  will be made.
     */
  sk_sp<T>& operator=(sk_sp<T>&& that)
  {
    this->reset(that.release());
    return *this;
  }
  template <typename U, typename  = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
  sk_sp<T>& operator=(sk_sp<U>&& that)
  {
    this->reset(that.release());
    return *this;
  }
  T& operator*() const
  {
    SkASSERT(this->get() != nullptr);
    return *this->get();
  }
  operator bool() const
  {
    return this->get() != nullptr;
  }
  T* get() const
  {
    return fPtr;
  }
  T* operator->() const
  {
    return fPtr;
  }
    /**
     *  Adopt the new bare pointer, and call unref() on any previously held object (if not null).
     *  No call to ref() will be made.
     */
  void reset(T* ptr = nullptr)
  {
    T* oldPtr = fPtr;
    fPtr = ptr;
    SkSafeUnref(oldPtr);
  }
    /**
     *  Return the bare pointer, and set the internal object pointer to nullptr.
     *  The caller must assume ownership of the object, and manage its reference count directly.
     *  No call to unref() will be made.
     */
  T* SK_WARN_UNUSED_RESULT release()
  {
    T* ptr = fPtr;
    fPtr = nullptr;
    return ptr;
  }
  void swap(sk_sp<T>& that)
  {
    using std::swap;
    swap(fPtr, that.fPtr);
  }
private:
  T* fPtr;
};
template <typename T>
inline void swap(sk_sp<T>& a, sk_sp<T>& b)
{
  a.swap(b);
}
template <typename T, typename U>
inline bool operator==(const sk_sp<T>& a, const sk_sp<U>& b)
{
  return a.get() == b.get();
}
template <typename T>
inline bool operator==(const sk_sp<T>& a, std::nullptr_t)
{
  return !a;
}
template <typename T>
inline bool operator==(std::nullptr_t, const sk_sp<T>& b)
{
  return !b;
}
template <typename T, typename U>
inline bool operator!=(const sk_sp<T>& a, const sk_sp<U>& b)
{
  return a.get() != b.get();
}
template <typename T>
inline bool operator!=(const sk_sp<T>& a, std::nullptr_t)
{
  return static_cast<bool>(a);
}
template <typename T>
inline bool operator!=(std::nullptr_t, const sk_sp<T>& b)
{
  return static_cast<bool>(b);
}
template <typename C, typename CT, typename T>
auto operator<<(std::basic_ostream<C, CT>& os, const sk_sp<T>& sp)
{
  return os << sp.get();
}
template <typename T, typename... Args>
sk_sp<T> sk_make_sp(Args&& args)
{
  return sk_sp<T>(new T(std::forward<Args>(args)));
}
/*
 *  Returns a sk_sp wrapping the provided ptr AND calls ref on it (if not null).
 *
 *  This is different than the semantics of the constructor for sk_sp, which just wraps the ptr,
 *  effectively "adopting" it.
 */
template <typename T>
sk_sp<T> sk_ref_sp(T* obj)
{
  return sk_sp<T>(SkSafeRef(obj));
}
template <typename T>
sk_sp<T> sk_ref_sp(const T* obj)
{
  return sk_sp<T>(const_cast<T*>(SkSafeRef(obj)));
}
#endif
