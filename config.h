#pragma once

#ifndef CONFIG_H_
#define CONFIG_H_

#define CONFIG_JSON "Config\\image_detail.json"
#define PLACES  "Readable\\places205.txt"
#define ATTRIBUTE "Readable\\SUN_attribute.txt"

#define TRAIN14_CLASS "Readable\\PredictData\\Class\\train2014_label_mini.txt"
#define VAL14_CLASS "Readable\\PredictData\\Class\\val2014_label_mini.txt"
#define TEST15_CLASS "Readable\\PredictData\\Class\\test2015_label_mini.txt"
#define TRAIN14_ATTR "Readable\\PredictData\\Attribute\\train2014_label_mini.txt"
#define VAL14_ATTR "Readable\\PredictData\\Attribute\\val2014_label_mini.txt"
#define TEST15_ATTR "Readable\\PredictData\\Attribute\\test2015_label_mini.txt"

#define SAMPLEROOT "Readable\\samples\\"
#define SAVEROOT "Annotation\\"

#ifdef DEBUG
#define TRAIN14 "train2014"
#define VAL14 "val2014"
#define TEST15 "test2015"
#define SAVE_TRAIN "Annotation\\train2014.json"
#define SAVE_VAL "Annotation\\val2014.json"
#define SAVE_TEST "Annotation\\test2015.json"

#else // Release 模式，正式发布时加_n
#define TRAIN 0
#define VAL 1
#define TEST 2
#define SAVE_TRAIN "Annotation\\train2014.json"
#define SAVE_VAL "Annotation\\val2014.json"
#define SAVE_TEST "Annotation\\test2015.json"
#endif

#endif