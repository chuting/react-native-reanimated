---
id: useAnimatedSensor
title: useAnimatedSensor
sidebar_label: useAnimatedSensor
---

:::tip

useAnimatedSensor is available since v2.5.0

:::

With the `useAnimatedSensor` hook, you can easily create cool interactive animations based on data from sensors in the device such as gyroscope, accelerometer etc.

```js
useAnimatedSensor(sensorType: [SensorType], config?: [UserConfig]) -> [AnimatedSensor]
```

### Arguments

#### `sensorType` - [[SensorType](#sensortype-enum)]

You can select the sensor available in [[SensorType](#sensortype-enum)] enum.

#### `config` - [[UserConfig](#userconfig-object)]

Optionally, you can pass configuration to customize the sensor behavior.

### Returns

Hook `useAnimatedSensor` returns an instance of [[AnimatedSensor](#animatedsensor-object)];

### Types

#### `AnimatedSensor: [object]`

Properties:

- `sensor`: [[SharedValue](../../api/hooks/useSharedValue)] contains [[3DVector](#3dvector-object)] or [[RotationVector](#rotationvector-object)] or `null`  
  contains actual sensor measurements as a shared value
- `unregister: [function]`  
  allows you to stop listening to sensor updates
- `isAvailable: [boolean]`  
  the flag contains information on the availability of sensors in a device
- `config`: [[UserConfig](#userconfig-object)]  
  the configuration provided by a user

#### `SensorType: [enum]`

`SensorType` is an enum that contains possibly supported sensors.
Values:

- `ACCELEROMETER`  
  measurements output as [[3DVector](#3dvector-object)]. Measured in m/s², excluding gravity.
- `GYROSCOPE`  
  measurements output as [[3DVector](#3dvector-object)]. Measured in rad/s.
- `GRAVITY`  
  measurements output as [[3DVector](#3dvector-object)]. Measured in m/s².
- `MAGNETIC_FIELD`  
  measurements output as [[3DVector](#3dvector-object)]. Measured in μT.
- `ROTATION`  
  measurements output as [[RotationVector](#rotationvector-object)]. [qx, qy, qz, qw] is a normalized quaternion. [yaw, pitch, roll] are rotations measured in radians along respective axes. We follow the iOS [convention](https://developer.apple.com/documentation/coremotion/getting_processed_device-motion_data/understanding_reference_frames_and_device_attitude).

#### `UserConfig: [object]`

Properties:

- `interval: [number | auto]` - interval in milliseconds between shared value updates. Pass `'auto'` to select interval based on device frame rate. Default: `'auto'`.
- `iosReferenceFrame: [[IOSReferenceFrame](#iosreferenceframe-enum)]` - reference frame to use on iOS. Default: `Auto`.
- `adjustToInterfaceOrientation: [boolean]` - whether to adjust measurements to the current interface orientation. For example, in the landscape orientation axes x and y may need to be reversed when drawn on the screen. It's `true` by default.

#### `IOSReferenceFrame: [enum]`

`IOSReferenceFrame` is an enum describing reference frame to use on iOS. It follows Apple's [documentation](https://developer.apple.com/documentation/coremotion/cmattitudereferenceframe). Possible values:

- `XArbitraryZVertical`
- `XArbitraryCorrectedZVertical`
- `XMagneticNorthZVertical`
- `XTrueNorthZVertical`
- `Auto` - on devices without magnetometer (for example iPods) `XArbitraryZVertical`, on devices with magnetometer `XArbitraryCorrectedZVertical`

#### `3DVector: [object]`

Properties:

- `x: number`
- `y: number`
- `z: number`
- `interfaceOrientation: [[InterfaceOrientation](#interfaceorientation-enum)]`

#### `RotationVector: [object]`

Properties:

- `qw: number`
- `qx: number`
- `qy: number`
- `qz: number`
- `yaw: number`
- `pitch: number`
- `roll: number`
- `interfaceOrientation: [[InterfaceOrientation](#interfaceorientation-enum)]`

#### `InterfaceOrientation: [enum]`

Values:

- `ROTATION_0` - default rotation on Android, portrait orientation on iOS
- `ROTATION_90` - 90 degrees rotation on Android, landscape right orientation on iOS (landscape and home button on the right)
- `ROTATION_180` - 180 degrees rotation on Android, upside down orientation on iOS
- `ROTATION_270` - 270 degrees rotation on Android, landscape left orientation on iOS (landscape and home button on the left)

### Example

```js
function UseAnimatedSensorExample() {
  const animatedSensor = useAnimatedSensor(SensorType.ROTATION, {
    interval: 10,
  }); // <- initialization
  const style = useAnimatedStyle(() => {
    const yaw = Math.abs(animatedSensor.sensor.value.yaw);
    const pitch = Math.abs(animatedSensor.sensor.value.pitch);
    return {
      height: withTiming(yaw * 200 + 20, { duration: 100 }), // <- usage
      width: withTiming(pitch * 200 + 20, { duration: 100 }), // <- usage
    };
  });

  return (
    <View style={{ flex: 1, justifyContent: 'center', alignItems: 'center' }}>
      <Animated.View style={[{ backgroundColor: 'black' }, style]} />
    </View>
  );
}
```

### Live example

<video src="https://user-images.githubusercontent.com/36106620/158634922-eaad656e-c837-44d5-8d51-8e7fa27c5a16.mp4" controls="controls" muted="muted" width="400"></video>

### Tips

:::caution

On iOS, if you want to read sensor data you need to enable location services on your device (`Settings > Privacy > Location Services`).

:::
