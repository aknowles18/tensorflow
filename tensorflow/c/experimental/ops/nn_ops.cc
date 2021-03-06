
/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include "tensorflow/c/experimental/ops/nn_ops.h"

#include "tensorflow/core/platform/errors.h"

namespace tensorflow {
namespace ops {

// Softmax Loss given scores and labels, used by the SoftMaxLossGradient
Status SparseSoftmaxCrossEntropyLoss(
    AbstractContext* ctx, absl::Span<AbstractTensorHandle* const> inputs,
    absl::Span<AbstractTensorHandle*> outputs, const char* name) {
  AbstractOperationPtr sm_loss_op(ctx->CreateOperation());
  TF_RETURN_IF_ERROR(sm_loss_op->Reset("SparseSoftmaxCrossEntropyWithLogits",
                                       /*raw_device_name=*/nullptr));

  if (isa<tracing::TracingOperation>(sm_loss_op.get())) {
    TF_RETURN_IF_ERROR(
        dyn_cast<tracing::TracingOperation>(sm_loss_op.get())->SetOpName(name));
  }

  TF_RETURN_IF_ERROR(sm_loss_op->AddInput(inputs[0]));  // input scores
  TF_RETURN_IF_ERROR(sm_loss_op->AddInput(inputs[1]));  // labels

  // Outputs will contain: [loss_vals, gradients].
  int num_retvals = 2;
  TF_RETURN_IF_ERROR(sm_loss_op->Execute(outputs, &num_retvals));
  return Status::OK();
}

// Computes Relu gradient given input features
Status ReluGrad(AbstractContext* ctx,
                absl::Span<AbstractTensorHandle* const> inputs,
                absl::Span<AbstractTensorHandle*> outputs, const char* name) {
  AbstractOperationPtr relugrad_op(ctx->CreateOperation());
  TF_RETURN_IF_ERROR(
      relugrad_op->Reset("ReluGrad", /*raw_device_name=*/nullptr));

  if (isa<tracing::TracingOperation>(relugrad_op.get())) {
    TF_RETURN_IF_ERROR(dyn_cast<tracing::TracingOperation>(relugrad_op.get())
                           ->SetOpName(name));
  }

  TF_RETURN_IF_ERROR(relugrad_op->AddInput(inputs[0]));  // upstream grads
  TF_RETURN_IF_ERROR(relugrad_op->AddInput(inputs[1]));  // relu inputs

  int num_retvals = 1;
  TF_RETURN_IF_ERROR(relugrad_op->Execute(outputs, &num_retvals));
  return Status::OK();
}

}  // namespace ops
}  // namespace tensorflow
