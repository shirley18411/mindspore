/**
 * Copyright 2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "internal/src/kernel/fp32_grad/arithmetic_self_grad.h"
#include "internal/src/kernel/common/common_infershape.h"
#include "internal/include/errorcode.h"
#include "internal/include/ms_tensor.h"
#include "internal/src/lite_log.h"
#include "nnacl/fp32/arithmetic_self.h"
#include "nnacl/fp32/arithmetic.h"

int DoArithmeticSelfGradInferShape(const TensorPtrVector &in_tensors, const TensorPtrVector &out_tensors,
                                   OpParameter *param) {
  return DoCommonInferShape(in_tensors, out_tensors);
}

int DoArithmeticSelfGrad(const TensorPtrVector &in_tensors, const TensorPtrVector &out_tensors, Node *node,
                         mindspore::lite::Allocator *allocator) {
  size_t data_size = in_tensors[0]->ElementsNum();
  OpParameter *param = node->primitive_;
  float *dy_data = reinterpret_cast<float *>(in_tensors[0]->data_);
  float *x_data = reinterpret_cast<float *>(in_tensors[1]->data_);
  float *dx_data = reinterpret_cast<float *>(out_tensors[0]->data_);
  int ret;
  if (param->type_ == KernelType::KernelType_LogGrad) {
    ret = ElementDiv(dy_data, x_data, dx_data, data_size);
  } else if (param->type_ == KernelType::KernelType_NegGrad) {
    ret = ElementNegative(dy_data, dx_data, data_size);
  } else {
    LITE_ERROR_LOG("Unsupport kernel type: %d", param->type_);
    return RET_PARAM_INVALID;
  }
  if (ret != NNACL_OK) {
    LITE_ERROR_LOG("do arithmetic %d fail!ret: %d", param->type_, ret);
    return RET_ERROR;
  }
  return RET_OK;
}
