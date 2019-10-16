/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef SkFontMgr_indirect_DEFINED
#  define SkFontMgr_indirect_DEFINED
#  include "include/core/SkFontMgr.h"
#  include "include/core/SkRefCnt.h"
#  include "include/core/SkTypeface.h"
#  include "include/core/SkTypes.h"
#  include "include/ports/SkRemotableFontMgr.h"
#  include "include/private/SkMutex.h"
#  include "include/private/SkOnce.h"
#  include "include/private/SkTArray.h"
class SkData;
class SkFontStyle;
class SkStreamAsset;
class SkString;
class SK_API SkFontMgr_Indirect : public SkFontMgr
{
public:
  SkFontMgr_Indirect(sk_sp<SkFontMgr> impl, sk_sp<SkRemotableFontMgr> proxy)
    : fImpl(std::move(impl))
    , fProxy(std::move(proxy))
  {
  }
protected:
  int onCountFamilies() const override;
  void onGetFamilyName(int index, SkString* familyName) const override;
  SkFontStyleSet* onCreateStyleSet(int index) const override;
  SkFontStyleSet* onMatchFamily(const char familyName[]) const override;
  SkTypeface* onMatchFamilyStyle(const char familyName[], const SkFontStyle& fontStyle) const override;
  SkTypeface* onMatchFamilyStyleCharacter(const char familyName[], const SkFontStyle&, const char* bcp47[], int bcp47Count, SkUnichar character) const override;
  SkTypeface* onMatchFaceStyle(const SkTypeface* familyMember, const SkFontStyle& fontStyle) const override;
  sk_sp<SkTypeface> onMakeFromStreamIndex(std::unique_ptr<SkStreamAsset>, int ttcIndex) const override;
  sk_sp<SkTypeface> onMakeFromFile(const char path[], int ttcIndex) const override;
  sk_sp<SkTypeface> onMakeFromData(sk_sp<SkData>, int ttcIndex) const override;
  sk_sp<SkTypeface> onLegacyMakeTypeface(const char familyName[], SkFontStyle) const override;
private:
  SkTypeface* createTypefaceFromFontId(const SkFontIdentity& fontId) const;
  sk_sp<SkFontMgr> fImpl;
  sk_sp<SkRemotableFontMgr> fProxy;
  struct DataEntry
  {
    uint32_t fDataId;
    uint32_t fTtcIndex;
    SkTypeface* fTypeface;
    DataEntry()
    {
    }
    DataEntry(DataEntry&& that)
      : fDataId(that.fDataId)
      , fTtcIndex(that.fTtcIndex)
      , fTypeface(that.fTypeface)
    {
      SkDEBUGCODE(that.fDataId = SkFontIdentity::kInvalidDataId;)
      SkDEBUGCODE(that.fTtcIndex = 0xbbadbeef;)
      that.fTypeface = nullptr;
    }
    ~DataEntry()
    {
      if (fTypeface)
      {
        fTypeface->weak_unref();
      }
    }
  };
    /**
     *  This cache is essentially { dataId: { ttcIndex: typeface } }
     *  For data caching we want a mapping from data id to weak references to
     *  typefaces with that data id. By storing the index next to the typeface,
     *  this data cache also acts as a typeface cache.
     */
  mutable SkTArray<DataEntry> fDataCache;
  mutable SkMutex fDataCacheMutex;
  friend class SkStyleSet_Indirect;
};
#endif
