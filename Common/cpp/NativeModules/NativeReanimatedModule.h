#pragma once

#ifdef RCT_NEW_ARCH_ENABLED
#include <react/renderer/uimanager/UIManager.h>
#endif

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "AnimatedSensorModule.h"
#include "EventHandlerRegistry.h"
#include "JSScheduler.h"
#include "LayoutAnimationsManager.h"
#include "NativeReanimatedModuleSpec.h"
#include "PlatformDepMethodsHolder.h"
#include "RuntimeDecorator.h"
#include "SingleInstanceChecker.h"
#include "UIScheduler.h"

#ifdef RCT_NEW_ARCH_ENABLED
#include "PropsRegistry.h"
#endif

namespace reanimated {

using PropObtainerFunction =
    std::function<jsi::Value(jsi::Runtime &, const int, const jsi::String &)>;
using RequestRenderFunction =
    std::function<void(std::function<void(double)> &, jsi::Runtime &)>;

class NativeReanimatedModule : public NativeReanimatedModuleSpec {
 public:
  NativeReanimatedModule(
      jsi::Runtime &rnRuntime,
      const std::shared_ptr<CallInvoker> &jsInvoker,
      const std::shared_ptr<UIScheduler> &uiScheduler,
#ifdef RCT_NEW_ARCH_ENABLED
  // nothing
#else
      PropObtainerFunction propObtainer,
#endif
      PlatformDepMethodsHolder platformDepMethodsHolder);

  ~NativeReanimatedModule();

  void installValueUnpacker(
      jsi::Runtime &rt,
      const jsi::Value &valueUnpackerCode) override;

  jsi::Value makeShareableClone(
      jsi::Runtime &rt,
      const jsi::Value &value,
      const jsi::Value &shouldRetainRemote) override;

  jsi::Value makeSynchronizedDataHolder(
      jsi::Runtime &rt,
      const jsi::Value &initialShareable) override;
  jsi::Value getDataSynchronously(
      jsi::Runtime &rt,
      const jsi::Value &synchronizedDataHolderRef) override;
  void updateDataSynchronously(
      jsi::Runtime &rt,
      const jsi::Value &synchronizedDataHolderRef,
      const jsi::Value &newData);

  void scheduleOnUI(jsi::Runtime &rt, const jsi::Value &worklet) override;
  void scheduleOnBackground(
      jsi::Runtime &rt,
      const jsi::Value &runtime,
      const jsi::Value &worklet) override;
  void scheduleOnJS(
      jsi::Runtime &rt,
      const jsi::Value &remoteFun,
      const jsi::Value &argsValue);

  jsi::Value registerEventHandler(
      jsi::Runtime &rt,
      const jsi::Value &worklet,
      const jsi::Value &eventName,
      const jsi::Value &emitterReactTag) override;
  void unregisterEventHandler(
      jsi::Runtime &rt,
      const jsi::Value &registrationId) override;

  jsi::Value getViewProp(
      jsi::Runtime &rt,
      const jsi::Value &viewTag,
      const jsi::Value &propName,
      const jsi::Value &callback) override;

  jsi::Value enableLayoutAnimations(jsi::Runtime &rt, const jsi::Value &config)
      override;
  jsi::Value configureProps(
      jsi::Runtime &rt,
      const jsi::Value &uiProps,
      const jsi::Value &nativeProps) override;
  jsi::Value configureLayoutAnimation(
      jsi::Runtime &rt,
      const jsi::Value &viewTag,
      const jsi::Value &type,
      const jsi::Value &sharedTransitionTag,
      const jsi::Value &config) override;

  void onRender(double timestampMs);

  bool isAnyHandlerWaitingForEvent(
      const std::string &eventName,
      const int emitterReactTag);

  void requestAnimationFrame(jsi::Runtime &rt, const jsi::Value &callback);
  void maybeRequestRender();

  bool handleEvent(
      const std::string &eventName,
      const int emitterReactTag,
      const jsi::Value &payload,
      double currentTime);

#ifdef RCT_NEW_ARCH_ENABLED
  bool handleRawEvent(const RawEvent &rawEvent, double currentTime);

  void updateProps(jsi::Runtime &rt, const jsi::Value &operations);

  void removeFromPropsRegistry(jsi::Runtime &rt, const jsi::Value &viewTags);

  void performOperations();

  void dispatchCommand(
      jsi::Runtime &rt,
      const jsi::Value &shadowNodeValue,
      const jsi::Value &commandNameValue,
      const jsi::Value &argsValue);

  jsi::Value measure(jsi::Runtime &rt, const jsi::Value &shadowNodeValue);

  void setUIManager(std::shared_ptr<UIManager> uiManager);

  void setPropsRegistry(std::shared_ptr<PropsRegistry> propsRegistry);
#endif

  jsi::Value registerSensor(
      jsi::Runtime &rt,
      const jsi::Value &sensorType,
      const jsi::Value &interval,
      const jsi::Value &iosReferenceFrame,
      const jsi::Value &sensorDataContainer) override;
  void unregisterSensor(jsi::Runtime &rt, const jsi::Value &sensorId) override;

  void cleanupSensors();

  jsi::Value subscribeForKeyboardEvents(
      jsi::Runtime &rt,
      const jsi::Value &keyboardEventContainer,
      const jsi::Value &isStatusBarTranslucent) override;
  void unsubscribeFromKeyboardEvents(
      jsi::Runtime &rt,
      const jsi::Value &listenerId) override;

  inline LayoutAnimationsManager &layoutAnimationsManager() {
    return layoutAnimationsManager_;
  }

  inline jsi::Runtime &getUIRuntime() {
    return uiWorkletRuntime_->getRuntime();
  }

 private:
#ifdef RCT_NEW_ARCH_ENABLED
  bool isThereAnyLayoutProp(jsi::Runtime &rt, const jsi::Object &props);
#endif // RCT_NEW_ARCH_ENABLED

  const std::shared_ptr<JSScheduler> jsScheduler_;
  const std::shared_ptr<UIScheduler> uiScheduler_;

  std::shared_ptr<WorkletRuntime> uiWorkletRuntime_;

  std::unique_ptr<EventHandlerRegistry> eventHandlerRegistry_;
  RequestRenderFunction requestRender_;
  std::vector<std::shared_ptr<jsi::Value>> frameCallbacks_;
  volatile bool renderRequested_{false};
  PropObtainerFunction propObtainer_;
  std::function<void(double)> onRenderCallback_;
  AnimatedSensorModule animatedSensorModule_;
  LayoutAnimationsManager layoutAnimationsManager_;

#ifdef RCT_NEW_ARCH_ENABLED
  SynchronouslyUpdateUIPropsFunction synchronouslyUpdateUIPropsFunction_;

  std::shared_ptr<UIManager> uiManager_;

  // After app reload, surfaceId on iOS is still 1 but on Android it's 11.
  // We can store surfaceId of the most recent ShadowNode as a workaround.
  SurfaceId surfaceId_ = -1;

  std::vector<std::pair<ShadowNode::Shared, std::unique_ptr<jsi::Value>>>
      operationsInBatch_; // TODO: refactor std::pair to custom struct

  std::shared_ptr<PropsRegistry> propsRegistry_;

  std::vector<Tag> tagsToRemove_; // from `propsRegistry_`
#else
  ConfigurePropsFunction configurePropsPlatformFunction_;
  UpdatePropsFunction updatePropsFunction_;
#endif

  std::unordered_set<std::string> nativePropNames_; // filled by configureProps

  KeyboardEventSubscribeFunction subscribeForKeyboardEventsFunction_;
  KeyboardEventUnsubscribeFunction unsubscribeFromKeyboardEventsFunction_;

#ifdef DEBUG
  SingleInstanceChecker<NativeReanimatedModule> singleInstanceChecker_;
#endif
};

} // namespace reanimated
