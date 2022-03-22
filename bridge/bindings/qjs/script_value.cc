/*
 * Copyright (C) 2021 Alibaba Inc. All rights reserved.
 * Author: Kraken Team.
 */

#include "script_value.h"
#include "native_string_utils.h"
#include "qjs_engine_patch.h"
#include <vector>
#include "core/executing_context.h"

namespace kraken {

ScriptValue ScriptValue::createErrorObject(JSContext* ctx, const char* errmsg) {
  JS_ThrowInternalError(ctx, "%s", errmsg);
  JSValue errorObject = JS_GetException(ctx);
  ScriptValue result = ScriptValue(ctx, errorObject);
  JS_FreeValue(ctx, errorObject);
  return result;
}

ScriptValue ScriptValue::createJSONObject(JSContext* ctx, const char* jsonString, size_t length) {
  JSValue jsonValue = JS_ParseJSON(ctx, jsonString, length, "");
  ScriptValue result = ScriptValue(ctx, jsonValue);
  JS_FreeValue(ctx, jsonValue);
  return result;
}

ScriptValue ScriptValue::Empty(JSContext* ctx) {
  return ScriptValue(ctx);
}

JSValue ScriptValue::ToQuickJS() const {
  return value_;
}

ScriptValue ScriptValue::ToJSONStringify(ExceptionState* exception) {
  JSValue stringifyedValue = JS_JSONStringify(ctx_, value_, JS_NULL, JS_NULL);
  ScriptValue result = ScriptValue(ctx_);
  // JS_JSONStringify may return JS_EXCEPTION if object is not valid. Return JS_EXCEPTION and let quickjs to handle it.
  if (JS_IsException(stringifyedValue)) {
    exception->ThrowException(ctx_, stringifyedValue);
  } else {
    result = ScriptValue(ctx_, stringifyedValue);
  }

  return result;
}

std::unique_ptr<NativeString> ScriptValue::toNativeString() {
  return jsValueToNativeString(ctx_, value_);
}

std::string ScriptValue::toCString() {
  return jsValueToStdString(ctx_, value_);
}

bool ScriptValue::IsException() {
  return JS_IsException(value_);
}

bool ScriptValue::IsEmpty() {
  return JS_IsNull(value_) || JS_IsUndefined(value_);
}

void ScriptValue::Trace(GCVisitor* visitor) {
  visitor->Trace(value_);
}

}  // namespace kraken
