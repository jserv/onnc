//===- ComputeOperator.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_H
#define ONNC_IR_COMPUTE_OPERATOR_H
#include <ostream>
#include <onnx/common/ir.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/ADT/Bits/DigraphNode.h>
#include <onnc/IR/Compute/Define.h>
#include <onnc/IR/ComputeOperand.h>
#include <onnc/IR/ComputeMemOperand.h>
#include <onnc/IR/Compute/Value.h>
#include <onnc/Support/IOStream.h>
#include <vector>

namespace onnc {

class ComputeVisitor;

/** \class ComputeOperator
 *
 *  ComputeOperator connects ONNX and machine ABI.
 */
class ComputeOperator : public onnc::Define,
                        public DigraphNode<ComputeOperator, ComputeOperand>
{
public:
  using GraphOperator = ::onnx::Node;

  typedef int64_t Opcode;

public:
  ComputeOperator(StringRef pName)
    : onnc::Define(pName),
      m_OpCode(0), m_Inputs(), m_Outputs(), m_GraphOperators() {
  }

  void setOpcode(Opcode pOP) { m_OpCode = pOP; }

  Opcode getOpCode() const { return m_OpCode; }

  /// Connect one compute operator to a graph operator @ref pGraphOperator
  /// One compute operator may connect to multiple graph operators.
  void connect(const GraphOperator& pGraphOperator);

  unsigned int getNumOfGraphOperators() const { return m_GraphOperators.size(); }

  unsigned int getNumOfInputs() const { return m_Inputs.size(); }

  unsigned int getNumOfOutputs() const { return m_Outputs.size(); }

  template<typename OpndType>
  void addInputs(OpndType& pOperand) { m_Inputs.push_back(&pOperand); }

  template<typename OpndType>
  void addOutputs(OpndType& pOperand) { m_Outputs.push_back(&pOperand); }

  /// Use covariant return type to override this function
  virtual onnc::Value* getInput(unsigned int pIdx) { return m_Inputs[pIdx]; }

  /// Use covariant return type to override this function
  virtual const onnc::Value* getInput(unsigned int pIdx) const { return m_Inputs[pIdx]; }

  /// Use covariant return type to override this function
  virtual onnc::Value* getOutput(unsigned int pIdx) { return m_Inputs[pIdx]; }

  /// Use covariant return type to override this function
  virtual const onnc::Value* getOutput(unsigned int pIdx) const { return m_Inputs[pIdx]; }

  /// display the operator
  void print(std::ostream& pOS) const = 0;

  /// redirect the printing to stderr
  void dump() const { return print(errs()); }

  /// entrance ramp for visitor
  virtual void accept(ComputeVisitor& pVisitor) = 0;

protected:
  typedef std::vector<const GraphOperator*>  GraphOperatorList;
  typedef std::vector<onnc::Value*> ValueList;
  typedef std::vector<ComputeOperand*> OperandList;

protected:
  Opcode m_OpCode;
  ValueList m_Inputs;
  ValueList m_Outputs;

  /// one compute operator may point to multiple graph operator.
  GraphOperatorList m_GraphOperators;
};

} // namespace of onnc

#endif
