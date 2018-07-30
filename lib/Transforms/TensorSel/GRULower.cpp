//===- GRULower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/GRULower.h>
#include <onnc/IR/Compute/GRU.h>
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// GRULower
//===----------------------------------------------------------------------===//
GRULower::GRULower()
{
}

GRULower::~GRULower()
{
}

int GRULower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("GRU"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
GRULower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
  // check input/output number
  if (pNode.inputs().size() < 3 || 6 < pNode.inputs().size())
    return nullptr;

  if (pNode.outputs().size() < 0 || 2 < pNode.outputs().size())
    return nullptr;

  // check input/output name
  for (::onnx::Value* xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  for (::onnx::Value* xv : pNode.outputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  // check default attributes
  

  // create operators
  onnc::GRU* op = pGraph.addOperator<onnc::GRU>();

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("activation_alpha")))
    op->setActivationAlpha(pNode.fs(::onnx::Symbol("activation_alpha")));
  if (pNode.hasAttribute(::onnx::Symbol("activation_beta")))
    op->setActivationBeta(pNode.fs(::onnx::Symbol("activation_beta")));
  if (pNode.hasAttribute(::onnx::Symbol("activations")))
    op->setActivations(pNode.ss(::onnx::Symbol("activations")));
  if (pNode.hasAttribute(::onnx::Symbol("clip")))
    op->setClip(pNode.f(::onnx::Symbol("clip")));
  if (pNode.hasAttribute(::onnx::Symbol("direction")))
    op->setDirection(pNode.s(::onnx::Symbol("direction")));
  if (pNode.hasAttribute(::onnx::Symbol("hidden_size")))
    op->setHiddenSize(pNode.i(::onnx::Symbol("hidden_size")));
  if (pNode.hasAttribute(::onnx::Symbol("linear_before_reset")))
    op->setLinearBeforeReset(pNode.i(::onnx::Symbol("linear_before_reset")));

  // set input/output
  for (::onnx::Value* xv : pNode.inputs()) {
    onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(xv->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pGraph, *xv);
    op->addInput(*tensor);
  }

  for (::onnx::Value* xv : pNode.outputs()) {
    onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(xv->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pGraph, *xv);
    op->addOutput(*tensor);
  }
  return op;
}
