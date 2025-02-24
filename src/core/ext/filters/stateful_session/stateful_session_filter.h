//
// Copyright 2022 gRPC authors.
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
//

#ifndef GRPC_CORE_EXT_FILTERS_STATEFUL_SESSION_STATEFUL_SESSION_FILTER_H
#define GRPC_CORE_EXT_FILTERS_STATEFUL_SESSION_STATEFUL_SESSION_FILTER_H

#include <grpc/support/port_platform.h>

#include <stddef.h>

#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "absl/types/optional.h"

#include "src/core/lib/channel/channel_args.h"
#include "src/core/lib/channel/channel_fwd.h"
#include "src/core/lib/channel/promise_based_filter.h"
#include "src/core/lib/gprpp/unique_type_name.h"
#include "src/core/lib/promise/arena_promise.h"
#include "src/core/lib/transport/transport.h"

namespace grpc_core {

UniqueTypeName XdsOverrideHostTypeName();

// A filter to provide cookie-based stateful session affinity.
class StatefulSessionFilter : public ChannelFilter {
 public:
  static const grpc_channel_filter kFilter;

  static absl::StatusOr<StatefulSessionFilter> Create(
      const ChannelArgs& args, ChannelFilter::Args filter_args);

  // Construct a promise for one call.
  ArenaPromise<ServerMetadataHandle> MakeCallPromise(
      CallArgs call_args, NextPromiseFactory next_promise_factory) override;

 private:
  explicit StatefulSessionFilter(ChannelFilter::Args filter_args);

  absl::optional<absl::string_view> GetOverrideHostFromCookie(
      const ClientMetadataHandle& initial_metadata,
      absl::string_view cookie_name);

  // The relative index of instances of the same filter.
  const size_t index_;
  // Index of the service config parser.
  const size_t service_config_parser_index_;
};

}  // namespace grpc_core

#endif  // GRPC_CORE_EXT_FILTERS_STATEFUL_SESSION_STATEFUL_SESSION_FILTER_H
