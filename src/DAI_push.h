#ifndef DAI_PUSH
#define DAI_PUSH

#include <Python.h>
#include <iostream>
#include <vector>
#include <ctime>
#include "sort.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "image.h"

void iot_init();
void iot_talk_send(std::vector<person_sort_det> &person_sort_dets, double &timer);

#ifdef __cplusplus
}
#endif

#endif

