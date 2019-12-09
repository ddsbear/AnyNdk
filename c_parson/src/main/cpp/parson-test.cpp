#include <jni.h>
#include <string>
#include "parson.h"
#include "android/log.h"

#define  LOG_TAG    "dds_parson"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__ )


extern "C" JNIEXPORT jstring JNICALL
Java_com_dds_parson_Parson_test(
        JNIEnv *env,
        jclass clazz) {
    char *serialized_string = 0;
    // 包装一段jsonObject
    JSON_Value *root_value = json_value_init_object();

    // 设置简单参数
    json_object_set_string(json_object(root_value), "name", "大大帅");
    json_object_set_number(json_object(root_value), "age", 27);
    json_object_set_string(json_object(root_value), "occupation", "Programmer");

    // 设置内联参数
    json_object_dotset_string(json_object(root_value), "address.city", "Cupertino");

    // 设置数组
    json_object_dotset_value(json_object(root_value), "sub.habbit",
                             json_parse_string(R"(["dsdssd","dsdsd","sdsdsd"])"));
    json_object_dotset_value(json_object(root_value), "contact.emails",
                             json_parse_string(R"(["email@example.com","email2@example.com"])"));

    // 导入object
    JSON_Value *pJsonSub = json_value_init_object();
    json_object_set_value(json_object(root_value), "address", pJsonSub);
    json_object_set_string(json_object(pJsonSub), "dds", "ddssingsong");

    // 导入array
    pJsonSub = json_value_init_array();
    json_object_set_value(json_object(root_value), "links", pJsonSub);

    JSON_Value *pJsonSubSub = json_value_init_object();
    json_array_append_value(json_array(pJsonSub), pJsonSubSub);
    json_object_set_string(json_object(pJsonSubSub), "dds", "ddssingsong");
    json_object_set_string(json_object(pJsonSubSub), "address", "https://baidu.com");
    pJsonSubSub = json_value_init_object();
    json_object_set_string(json_object(pJsonSubSub), "dds", "ddssingsong");
    json_object_set_string(json_object(pJsonSubSub), "address", "https://baidu.com");

    serialized_string = json_serialize_to_string_pretty(root_value);
    json_value_free(root_value);

    return env->NewStringUTF(serialized_string);
}


extern "C"
JNIEXPORT jobject JNICALL
Java_com_dds_parson_Parson_parseJson(JNIEnv *env, jclass clazz, jstring json) {
    JSON_Value *root_value;
    char g_log_info[200];
    // 解析传入的json
    const char *cJson = env->GetStringUTFChars(json, JNI_FALSE);
    root_value = json_parse_string(cJson);

    // 获取根类型 object/array
    JSON_Value_Type jsonValueType = json_value_get_type(root_value);

    if (jsonValueType == JSONObject) {
        JSON_Object *jsonObject = json_value_get_object(root_value);

        // 获取其中一个参数
        const char *name = json_object_get_string(jsonObject, "name");
        sprintf(g_log_info, "%s", name);
        LOGD("name : %s", g_log_info);

        // 获取其中一个参数
        const char *occu = json_object_get_string(jsonObject, "occupation");
        sprintf(g_log_info, "%s", occu);
        LOGD("occupation : %s", g_log_info);

        // 反射类
        jclass userClass = env->FindClass("com/dds/parson/User");
        //反射构造方法
        jmethodID constructMethod = env->GetMethodID(userClass, "<init>", "()V");

        if (userClass == NULL || constructMethod == NULL) {
            return NULL;
        }
        // 构造对象
        jobject user = env->NewObject(userClass, constructMethod);
        // 设置参数
        jmethodID setName = env->GetMethodID(userClass, "setName", "(Ljava/lang/String;)V");
        jmethodID setOccu = env->GetMethodID(userClass, "setOccu", "(Ljava/lang/String;)V");
        jstring jName = env->NewStringUTF(name);
        env->CallVoidMethod(user, setName, jName);
        jstring jOccu = env->NewStringUTF(occu);
        env->CallVoidMethod(user, setOccu, jOccu);


        jmethodID setHabbit = env->GetMethodID(userClass, "setHabbits", "(Ljava/util/ArrayList;)V");
        if (setHabbit == NULL) {
            LOGD("setHabbit method is NULL");
            return NULL;
        }
        jclass listClass = env->FindClass("java/util/ArrayList");
        jmethodID list_init = env->GetMethodID(listClass, "<init>", "(I)V");
        jobject listObject = env->NewObject(listClass, list_init,4);
        jmethodID list_add = env->GetMethodID(listClass, "add",
                                              "(Ljava/lang/Object;)Z");
        if (listClass == NULL || list_init == NULL) {
            LOGD("listClass is NULL");
            return NULL;
        }
        if (list_add == NULL) {
            LOGD("add method is NULL");
            return NULL;
        }

        // 获取array并打印长度
        JSON_Array *habbits = json_object_dotget_array(jsonObject, "sub.habbit");
        size_t size = json_array_get_count(habbits);
        LOGD("size %d", (int) size);

        size_t a = 0;
        for (a = 0; a < size; a++) {
            const char *ha = json_array_get_string(habbits, a);
            sprintf(g_log_info, "%s", ha);
            LOGD("habbit %d: %s", a, g_log_info);
            jstring haa = env->NewStringUTF(ha);

            bool flag = env->CallBooleanMethod(listObject, list_add, haa);
            LOGD("flag:%d", flag);
        }
        env->CallVoidMethod(user, setHabbit, listObject);

        json_value_free(root_value);
        return user;

    }
    return NULL;


}

