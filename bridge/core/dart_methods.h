/*
 * Copyright (C) 2019-2022 The Kraken authors. All rights reserved.
 * Copyright (C) 2022-present The WebF authors. All rights reserved.
 */

#ifndef WEBF_DART_METHODS_H_
#define WEBF_DART_METHODS_H_

/// Functions implements at dart side, including timer, Rendering and module API.
/// Communicate via Dart FFI.

#include <memory>
#include <thread>
#include "foundation/native_string.h"
#include "foundation/native_value.h"

#if defined(_WIN32)
#define WEBF_EXPORT_C extern "C" __declspec(dllexport)
#define WEBF_EXPORT __declspec(dllexport)
#else
#define WEBF_EXPORT_C extern "C" __attribute__((visibility("default"))) __attribute__((used))
#define WEBF_EXPORT __attribute__((__visibility__("default")))
#endif

namespace webf {

using AsyncCallback = void (*)(void* callback_context, int32_t context_id, const char* errmsg);
using AsyncRAFCallback = void (*)(void* callback_context, int32_t context_id, double result, const char* errmsg);
using AsyncModuleCallback = NativeValue* (*)(void* callback_context,
                                             int32_t context_id,
                                             const char* errmsg,
                                             NativeValue* value);
using AsyncBlobCallback =
    void (*)(void* callback_context, int32_t context_id, const char* error, uint8_t* bytes, int32_t length);
typedef NativeValue* (*InvokeModule)(void* callback_context,
                                     int32_t context_id,
                                     SharedNativeString* moduleName,
                                     SharedNativeString* method,
                                     NativeValue* params,
                                     AsyncModuleCallback callback);
typedef void (*RequestBatchUpdate)(int32_t context_id);
typedef void (*ReloadApp)(int32_t context_id);
typedef int32_t (*SetTimeout)(void* callback_context, int32_t context_id, AsyncCallback callback, int32_t timeout);
typedef int32_t (*SetInterval)(void* callback_context, int32_t context_id, AsyncCallback callback, int32_t timeout);
typedef int32_t (*RequestAnimationFrame)(void* callback_context, int32_t context_id, AsyncRAFCallback callback);
typedef void (*ClearTimeout)(int32_t context_id, int32_t timerId);
typedef void (*CancelAnimationFrame)(int32_t context_id, int32_t id);
typedef void (*ToBlob)(void* callback_context,
                       int32_t context_id,
                       AsyncBlobCallback blobCallback,
                       void* element_ptr,
                       double devicePixelRatio);
typedef void (*OnJSError)(int32_t context_id, const char*);
typedef void (*OnJSLog)(int32_t context_id, int32_t level, const char*);
typedef void (*FlushUICommand)(int32_t context_id);
typedef void (
    *CreateBindingObject)(int32_t context_id, void* native_binding_object, int32_t type, void* args, int32_t argc);

using MatchImageSnapshotCallback = void (*)(void* callback_context, int32_t context_id, int8_t, const char* errmsg);
using MatchImageSnapshot = void (*)(void* callback_context,
                                    int32_t context_id,
                                    uint8_t* bytes,
                                    int32_t length,
                                    SharedNativeString* name,
                                    MatchImageSnapshotCallback callback);
using MatchImageSnapshotBytes = void (*)(void* callback_context,
                                         int32_t context_id,
                                         uint8_t* image_a_bytes,
                                         int32_t image_a_size,
                                         uint8_t* image_b_bytes,
                                         int32_t image_b_size,
                                         MatchImageSnapshotCallback callback);
using Environment = const char* (*)();

#if ENABLE_PROFILE
struct NativePerformanceEntryList {
  uint64_t* entries;
  int32_t length;
};
typedef NativePerformanceEntryList* (*GetPerformanceEntries)(int32_t);
#endif

struct MousePointer {
  int32_t context_id;
  double x;
  double y;
  double change;
  int32_t signal_kind;
  double delta_x;
  double delta_y;
};
using SimulatePointer =
    void (*)(void* ptr, MousePointer*, int32_t length, int32_t pointer, AsyncCallback async_callback);
using SimulateInputText = void (*)(SharedNativeString* nativeString);

class DartMethodPointer {
  DartMethodPointer() = delete;

 public:
  explicit DartMethodPointer(void* dart_isolate_context, const uint64_t* dart_methods, int32_t dartMethodsLength);
  NativeValue* invokeModule(void* callback_context,
                            int32_t context_id,
                            SharedNativeString* moduleName,
                            SharedNativeString* method,
                            NativeValue* params,
                            AsyncModuleCallback callback);

  void requestBatchUpdate(int32_t context_id);
  void reloadApp(int32_t context_id);
  int32_t setTimeout(void* callback_context, int32_t context_id, AsyncCallback callback, int32_t timeout);
  int32_t setInterval(void* callback_context, int32_t context_id, AsyncCallback callback, int32_t timeout);
  void clearTimeout(int32_t context_id, int32_t timerId);
  int32_t requestAnimationFrame(void* callback_context, int32_t context_id, AsyncRAFCallback callback);
  void cancelAnimationFrame(int32_t context_id, int32_t id);
  void toBlob(void* callback_context,
              int32_t context_id,
              AsyncBlobCallback blobCallback,
              void* element_ptr,
              double devicePixelRatio);
  void flushUICommand(int32_t context_id);
  void createBindingObject(int32_t context_id, void* native_binding_object, int32_t type, void* args, int32_t argc);

  void onJSError(int32_t context_id, const char*);
  void onJSLog(int32_t context_id, int32_t level, const char*);

  void matchImageSnapshot(void* callback_context,
                          int32_t context_id,
                          uint8_t* bytes,
                          int32_t length,
                          SharedNativeString* name,
                          MatchImageSnapshotCallback callback);

  void matchImageSnapshotBytes(void* callback_context,
                               int32_t context_id,
                               uint8_t* image_a_bytes,
                               int32_t image_a_size,
                               uint8_t* image_b_bytes,
                               int32_t image_b_size,
                               MatchImageSnapshotCallback callback);

  const char* environment();
  void simulatePointer(void* ptr, MousePointer*, int32_t length, int32_t pointer, AsyncCallback async_callback);
  void simulateInputText(SharedNativeString* nativeString);

  void SetOnJSError(OnJSError func);
  void SetMatchImageSnapshot(MatchImageSnapshot func);
  void SetMatchImageSnapshotBytes(MatchImageSnapshotBytes func);
  void SetEnvironment(Environment func);
  void SetSimulatePointer(SimulatePointer func);
  void SetSimulateInputText(SimulateInputText func);

 private:
  InvokeModule invoke_module_{nullptr};
  RequestBatchUpdate request_batch_update_{nullptr};
  ReloadApp reload_app_{nullptr};
  SetTimeout set_timeout_{nullptr};
  SetInterval set_interval_{nullptr};
  ClearTimeout clear_timeout_{nullptr};
  RequestAnimationFrame request_animation_frame_{nullptr};
  CancelAnimationFrame cancel_animation_frame_{nullptr};
  ToBlob to_blob_{nullptr};
  FlushUICommand flush_ui_command_{nullptr};
  CreateBindingObject create_binding_object_{nullptr};
  OnJSError on_js_error_{nullptr};
  OnJSLog on_js_log_{nullptr};
  MatchImageSnapshot match_image_snapshot_{nullptr};
  MatchImageSnapshotBytes match_image_snapshot_bytes_{nullptr};
  Environment environment_{nullptr};
  SimulatePointer simulate_pointer_{nullptr};
  SimulateInputText simulate_input_text_{nullptr};
};

}  // namespace webf

#endif
