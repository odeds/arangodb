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
/// @author Michael Hackstein
////////////////////////////////////////////////////////////////////////////////

#ifndef ARANGOD_AQL_TRAVERSAL_EXECUTOR_H
#define ARANGOD_AQL_TRAVERSAL_EXECUTOR_H

#include "Aql/ExecutionState.h"
#include "Aql/ExecutorInfos.h"
#include "Aql/InputAqlItemRow.h"
#include "Aql/TraversalStats.h"

namespace arangodb {

namespace traverser {
class Traverser;
}

namespace aql {

class InputAqlItemRow;
class OutputAqlItemRow;
class ExecutorInfos;
class SingleRowFetcher;

enum OutputName { VERTEX, EDGE, PATH };

class TraversalExecutorInfos : public ExecutorInfos {
 public:
  TraversalExecutorInfos(std::shared_ptr<std::unordered_set<RegisterId>> inputRegisters,
                         std::shared_ptr<std::unordered_set<RegisterId>> outputRegisters,
                         RegisterId nrInputRegisters, RegisterId nrOutputRegisters,
                         std::unordered_set<RegisterId> registersToClear,
                         std::unique_ptr<traverser::Traverser>&& traverser,
                         std::unordered_map<OutputName, RegisterId> registerMapping,
                         std::string const& fixedSource);

  TraversalExecutorInfos() = delete;

  TraversalExecutorInfos(TraversalExecutorInfos&&) = default;
  TraversalExecutorInfos(TraversalExecutorInfos const&) = delete;
  ~TraversalExecutorInfos() = default;

  traverser::Traverser& traverser();

  bool useVertexOutput() const;

  RegisterId vertexRegister() const;

  bool useEdgeOutput() const;

  RegisterId edgeRegister() const;

  bool usePathOutput() const;

  RegisterId pathRegister() const;

  std::string const& getFixedSource() const;

 private:
  std::unique_ptr<traverser::Traverser> _traverser;
  std::unordered_map<OutputName, RegisterId> _registerMapping;
  std::string const _fixedSource;
};

/**
 * @brief Implementation of Traversal Node
 */
class TraversalExecutor {
 public:
  using Fetcher = SingleRowFetcher;
  using Infos = TraversalExecutorInfos;
  using Stats = TraversalStats;

  TraversalExecutor() = delete;
  TraversalExecutor(TraversalExecutor&&) = default;
  TraversalExecutor(TraversalExecutor const&) = default;
  TraversalExecutor(Fetcher& fetcher, Infos&);
  ~TraversalExecutor();

  /**
   * @brief produce the next Row of Aql Values.
   *
   * @return ExecutionState, and if successful exactly one new Row of AqlItems.
   */
  std::pair<ExecutionState, Stats> produceRow(OutputAqlItemRow& output);

 private:
  /**
   * @brief compute the return state
   * @return DONE if traverser and remote are both done, HASMORE otherwise
   */
  ExecutionState computeState() const;

 private:
  Infos& _infos;
  Fetcher& _fetcher;
  InputAqlItemRow _input;
  ExecutionState _rowState;
  traverser::Traverser& _traverser;
};

}  // namespace aql
}  // namespace arangodb

#endif
