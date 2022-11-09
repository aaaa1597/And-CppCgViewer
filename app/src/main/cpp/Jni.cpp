#include <jni.h>
#include <string>
#include <mutex>

std::mutex gMtx;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_tks_cppcgviewer_Jni_onStart(JNIEnv *env, jclass clazz) {
    gMtx.lock();    /* onStart() → surfaceChanged()の完了までは、描画処理を動かさない */
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

#ifdef __cplusplus
}
#endif
