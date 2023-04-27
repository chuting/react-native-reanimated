import React from 'react';

import Animated, {
  useAnimatedRef,
  useAnimatedStyle,
  useSharedValue,
} from 'react-native-reanimated';
import type { SharedValue } from 'react-native-reanimated';
import { Button, StyleSheet, Text, View } from 'react-native';

export default function ScrollViewOffsetExample() {
  const aref = useAnimatedRef<Animated.ScrollView>();
  const scrollHandler: SharedValue<number> = useSharedValue<number>(0);

  useAnimatedStyle(() => {
    console.log(scrollHandler.value);
    return {};
  });

  const onButtonPress = () => {
    aref.current?.scrollTo({
      y: Math.random() * 2000,
      animated: true,
    });
  };

  return (
    <>
      <View style={styles.positionView}>
        <Text>Test</Text>
      </View>
      <View style={styles.divider} />
      <Button title="Scroll random" onPress={onButtonPress} />
      <Animated.ScrollView
        ref={aref}
        scrollViewOffset={scrollHandler}
        scrollEventThrottle={16}
        style={styles.scrollView}>
        {[...Array(100)].map((_, i) => (
          <Text key={i} style={styles.text}>
            {i}
          </Text>
        ))}
      </Animated.ScrollView>
    </>
  );
}

const styles = StyleSheet.create({
  positionView: {
    margin: 20,
    alignItems: 'center',
  },
  scrollView: {
    flex: 1,
    width: '100%',
  },
  text: {
    fontSize: 50,
    textAlign: 'center',
  },
  divider: {
    backgroundColor: 'black',
    height: 1,
  },
});
