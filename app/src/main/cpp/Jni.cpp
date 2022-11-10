#include <jni.h>
#include <string>
#include <vector>
#include <mutex>
#include <android/log.h>
#include <android/asset_manager_jni.h>

std::mutex gMtx;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jboolean JNICALL Java_com_tks_cppcgviewer_Jni_onStart(JNIEnv *env, jclass clazz, jobject assets,
                                                            jobjectArray modelnames, jobjectArray md2filenames,
                                                            jobjectArray texfilenames, jobjectArray vshfilenames,
                                                            jobjectArray fshfilenames,
                                                            jfloatArray rotxs, jfloatArray rotys, jfloatArray rotzs,
                                                            jfloatArray posxs, jfloatArray posys, jfloatArray poszs,
                                                            jfloatArray sclxs, jfloatArray sclys, jfloatArray sclzs) {
    gMtx.lock();    /* onStart() → surfaceChanged()の完了までは、描画処理を動かさない */

    /* 引数チェック(md2model) */
    jsize size0 = env->GetArrayLength(modelnames);
    jsize size1 = env->GetArrayLength(md2filenames);
    jsize size2 = env->GetArrayLength(texfilenames);
    jsize size3 = env->GetArrayLength(vshfilenames);
    jsize size4 = env->GetArrayLength(fshfilenames);
    jsize size5 = env->GetArrayLength(rotxs);
    jsize size6 = env->GetArrayLength(rotys);
    jsize size7 = env->GetArrayLength(rotzs);
    jsize size8 = env->GetArrayLength(posxs);
    jsize size9 = env->GetArrayLength(posys);
    jsize size10= env->GetArrayLength(posxs);
    jsize size11= env->GetArrayLength(sclxs);
    jsize size12= env->GetArrayLength(sclys);
    jsize size13= env->GetArrayLength(sclzs);
    /* 引数lengthが一致しなければ引数不正 */
    if(size0 != size1 || size0 != size2 || size0 != size3 || size0 != size4 || size0 != size5 || size0 != size6 || size0 != size7 || size0 != size8 || size0 != size9 || size0 != size10 || size0 != size11 || size0 != size12 || size0 != size13) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "引数不正 名称リストの数が合わない!!! modelname.size=%d md2filenames.size=%d texfilenames.size=%d vshfilenames.size=%d fshfilenames.size=%d %s %s(%d)", size0, size1, size2, size3, size4, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return false;
    }

    /* AAssetManager取得 */
    AAssetManager *assetMgr = AAssetManager_fromJava(env, assets);
    if (assetMgr == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "ERROR loading asset!!");
        return false;
    }

    /* 開始位置情報取得 */
    jfloat *rotxary = env->GetFloatArrayElements(rotxs, nullptr);
    jfloat *rotyary = env->GetFloatArrayElements(rotys, nullptr);
    jfloat *rotzary = env->GetFloatArrayElements(rotzs, nullptr);
    jfloat *posxary = env->GetFloatArrayElements(posxs, nullptr);
    jfloat *posyary = env->GetFloatArrayElements(posys, nullptr);
    jfloat *poszary = env->GetFloatArrayElements(poszs, nullptr);
    jfloat *sclxary = env->GetFloatArrayElements(sclxs, nullptr);
    jfloat *sclyary = env->GetFloatArrayElements(sclys, nullptr);
    jfloat *sclzary = env->GetFloatArrayElements(sclzs, nullptr);
    for(int lpct = 0; lpct < size0; lpct++) {
        /* jobjectArray -> jstring */
        jstring modelnamejstr   = (jstring)env->GetObjectArrayElement(modelnames  , lpct);
        jstring md2filenamejstr = (jstring)env->GetObjectArrayElement(md2filenames, lpct);
        jstring texfilenamejstr = (jstring)env->GetObjectArrayElement(texfilenames, lpct);
        jstring vshfilenamejstr = (jstring)env->GetObjectArrayElement(vshfilenames, lpct);
        jstring fshfilenamejstr = (jstring)env->GetObjectArrayElement(fshfilenames, lpct);

        /* 個別の開始位置情報取得 */
        jfloat rotx = rotxary[lpct];
        jfloat roty = rotyary[lpct];
        jfloat rotz = rotzary[lpct];
        jfloat posx = posxary[lpct];
        jfloat posy = posyary[lpct];
        jfloat posz = poszary[lpct];
        jfloat sclx = sclxary[lpct];
        jfloat scly = sclyary[lpct];
        jfloat sclz = sclzary[lpct];

        /* jstring -> char */
        const char *modelnamechar   = env->GetStringUTFChars(modelnamejstr  , nullptr);
        const char *md2filenamechar = env->GetStringUTFChars(md2filenamejstr, nullptr);
        const char *texfilenamechar = env->GetStringUTFChars(texfilenamejstr, nullptr);
        const char *vshfilenamechar = env->GetStringUTFChars(vshfilenamejstr, nullptr);
        const char *fshfilenamechar = env->GetStringUTFChars(fshfilenamejstr, nullptr);

        /* md2関連データ一括読込み */
        std::vector<std::pair<std::string, std::vector<char>>> wk = {{md2filenamechar, std::vector<char>()},
                                                                     {texfilenamechar, std::vector<char>()},
                                                                     {vshfilenamechar, std::vector<char>()},
                                                                     {fshfilenamechar, std::vector<char>()}};
        for(std::pair<std::string, std::vector<char>> &item : wk) {
            const std::string &filename = item.first;
            std::vector<char> &binbuf = item.second;

            /* AAsset::open */
            AAsset *assetFile = AAssetManager_open(assetMgr, filename.c_str(), AASSET_MODE_RANDOM);
            if (assetFile == nullptr) {
                __android_log_print(ANDROID_LOG_INFO, "aaaaa", "ERROR AAssetManager_open(%s) %s %s(%d)", filename.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
                return false;
            }
            /* 読込 */
            int size = AAsset_getLength(assetFile);
            binbuf.resize(size);
            AAsset_read(assetFile, binbuf.data(), size);
            /* AAsset::close */
            AAsset_close(assetFile);
        }

//        /* Md2model追加 */
//        gMd2Models.emplace(modelnamechar, Md2Model{ .mName=modelnamechar,
//                .mWkMd2BinData=std::move(wk[0].second),
//                .mWkTexBinData=std::move(wk[1].second),
//                /* shaderはデータを文字列に変換して格納 */
//                .mWkVshStrData=std::string(wk[2].second.begin(), wk[2].second.end()),
//                .mWkFshStrData=std::string(wk[3].second.begin(), wk[3].second.end())});

        /* char解放 */
        env->ReleaseStringUTFChars(modelnamejstr  , modelnamechar);
        env->ReleaseStringUTFChars(md2filenamejstr, md2filenamechar);
        env->ReleaseStringUTFChars(texfilenamejstr, texfilenamechar);
        env->ReleaseStringUTFChars(vshfilenamejstr, vshfilenamechar);
        env->ReleaseStringUTFChars(fshfilenamejstr, fshfilenamechar);

        /* jstring解放 */
        env->DeleteLocalRef(modelnamejstr);
        env->DeleteLocalRef(md2filenamejstr);
        env->DeleteLocalRef(texfilenamejstr);
        env->DeleteLocalRef(vshfilenamejstr);
        env->DeleteLocalRef(fshfilenamejstr);
    }
    /* jfloat解放 */
    env->ReleaseFloatArrayElements(rotxs, rotxary, 0);
    env->ReleaseFloatArrayElements(rotys, rotyary, 0);
    env->ReleaseFloatArrayElements(rotzs, rotzary, 0);
    env->ReleaseFloatArrayElements(posxs, posxary, 0);
    env->ReleaseFloatArrayElements(posys, posyary, 0);
    env->ReleaseFloatArrayElements(poszs, poszary, 0);
    env->ReleaseFloatArrayElements(sclxs, sclxary, 0);
    env->ReleaseFloatArrayElements(sclys, sclyary, 0);
    env->ReleaseFloatArrayElements(sclzs, sclzary, 0);

    return true;
}

JNIEXPORT void JNICALL Java_com_tks_cppcgviewer_Jni_surfaceCreated(JNIEnv *env, jclass clazz, jobject surface) {
}

JNIEXPORT void JNICALL Java_com_tks_cppcgviewer_Jni_surfaceChanged(JNIEnv *env, jclass clazz, jint width, jint height) {
    gMtx.unlock();    /* onStart() → surfaceChanged()の完了までは、描画処理を動かさない */
}

JNIEXPORT void JNICALL Java_com_tks_cppcgviewer_Jni_surfaceDestroyed(JNIEnv *env, jclass clazz) {
}

JNIEXPORT void JNICALL Java_com_tks_cppcgviewer_Jni_onStop(JNIEnv *env, jclass clazz) {
}

JNIEXPORT void JNICALL Java_com_tks_cppcgviewer_Jni_setScale(JNIEnv *env, jclass clazz, jfloat scale) {
}

JNIEXPORT void JNICALL Java_com_tks_cppcgviewer_Jni_setRotate(JNIEnv *env, jclass clazz, jfloat rotatex, jfloat rotatey) {
}

#ifdef __cplusplus
}
#endif
