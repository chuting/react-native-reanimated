#include "ReanimatedRuntime.h"

#include <cxxreact/MessageQueueThread.h>
#include <jsi/jsi.h>

#include <memory>
#include <utility>

#if JS_RUNTIME_HERMES
#include "ReanimatedHermesRuntime.h"
#elif JS_RUNTIME_V8
#include <v8runtime/V8RuntimeFactory.h>
#else
#if REACT_NATIVE_MINOR_VERSION >= 71
#include <jsc/JSCRuntime.h>
#else
#include <jsi/JSCRuntime.h>
#endif // REACT_NATIVE_MINOR_VERSION
#endif // JS_RUNTIME

namespace reanimated {

using namespace facebook;
using namespace react;

std::shared_ptr<jsi::Runtime> ReanimatedRuntime::make(
    jsi::Runtime &rnRuntime,
    const std::string &name) {
#if JS_RUNTIME_HERMES
  std::unique_ptr<facebook::hermes::HermesRuntime> runtime =
      facebook::hermes::makeHermesRuntime();
  return std::make_shared<ReanimatedHermesRuntime>(std::move(runtime), name);
#elif JS_RUNTIME_V8
  auto config = std::make_unique<rnv8::V8RuntimeConfig>();
  config->enableInspector = false;
  config->appName = name;
  return rnv8::createSharedV8Runtime(&rnRuntime, std::move(config));
#else
  return facebook::jsc::makeJSCRuntime();
#endif
}

} // namespace reanimated
