////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2018 ArangoDB GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Jan Christoph Uhde
////////////////////////////////////////////////////////////////////////////////

#ifndef ARANGOD_AQL_RETURN_EXECUTOR_H
#define ARANGOD_AQL_RETURN_EXECUTOR_H

#include "Aql/ExecutionState.h"
#include "Aql/ExecutorInfos.h"
#include "Aql/Stats.h"

namespace arangodb {
namespace transaction {
class Methods;
}

namespace aql {

template <bool>
class SingleRowFetcher;
class AqlItemMatrix;
class ExecutorInfos;
class NoStats;
class OutputAqlItemRow;
struct SortRegister;

class ReturnExecutorInfos : public ExecutorInfos {
 public:
  ReturnExecutorInfos(RegisterId inputRegister, RegisterId nrInputRegisters,
                      RegisterId nrOutputRegisters,
                      std::unordered_set<RegisterId> registersToClear,
                      bool doCount, bool returnInheritedResults);

  ReturnExecutorInfos() = delete;
  ReturnExecutorInfos(ReturnExecutorInfos&&) = default;
  ReturnExecutorInfos(ReturnExecutorInfos const&) = delete;
  ~ReturnExecutorInfos() = default;

  Variable const& inVariable() const { return *_inVariable; }

  RegisterId getInputRegisterId() const { return _inputRegisterId; }

  RegisterId getOutputRegisterId() const {
    // Should not be used with returnInheritedResults.
    TRI_ASSERT(!returnInheritedResults());
    return 0;
  }

  bool doCount() const { return _doCount; }

  bool returnInheritedResults() const { return _returnInheritedResults; }

 private:
  /// @brief the variable produced by Return
  Variable const* _inVariable;
  RegisterId _inputRegisterId;
  bool _doCount;
  bool _returnInheritedResults;
};

/**
 * @brief Implementation of Return Node
 */
template<bool passBlocksThrough>
class ReturnExecutor {
 public:
  struct Properties {
    static const bool preservesOrder = true;
    static const bool allowsBlockPassthrough = passBlocksThrough;
  };
  using Fetcher = SingleRowFetcher<Properties::allowsBlockPassthrough>;
  using Infos = ReturnExecutorInfos;
  using Stats = CountStats;

  ReturnExecutor(Fetcher& fetcher, ReturnExecutorInfos&);
  ~ReturnExecutor();

  /**
   * @brief produce the next Row of Aql Values.
   *
   * @return ExecutionState,
   *         if something was written output.hasValue() == true
   */
  std::pair<ExecutionState, Stats> produceRow(OutputAqlItemRow& output);

 private:
  ReturnExecutorInfos& _infos;
  Fetcher& _fetcher;
};
}  // namespace aql
}  // namespace arangodb

#endif
