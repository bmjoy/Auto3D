// Copyright (c) 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SOURCE_FUZZ_FUZZER_PASS_SPLIT_BLOCKS_
#define SOURCE_FUZZ_FUZZER_PASS_SPLIT_BLOCKS_

#include "source/fuzz/fuzzer_pass.h"

namespace spvtools {
namespace fuzz {

// A fuzzer pass for splitting blocks in the module, to create more blocks; this
// can be very useful for giving other passes a chance to apply.
class FuzzerPassSplitBlocks : public FuzzerPass {
 public:
  FuzzerPassSplitBlocks(opt::IRContext* ir_context, FactManager* fact_manager,
                        FuzzerContext* fuzzer_context,
                        protobufs::TransformationSequence* transformations);

  ~FuzzerPassSplitBlocks() override;

  void Apply() override;
};

}  // namespace fuzz
}  // namespace spvtools

#endif  // #define SOURCE_FUZZ_FUZZER_PASS_SPLIT_BLOCKS_
