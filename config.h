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


#define TRAIN14 "train2014"
#define VAL14 "val2014"
#define TEST15 "test2015"


#define TRAIN 0
#define VAL 1
#define TEST 2

#define TRAIN_NUM 82783
#define VAL_NUM 40504
#define TEST_NUM 81434


#endif