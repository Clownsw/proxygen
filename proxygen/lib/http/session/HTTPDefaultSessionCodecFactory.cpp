/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <proxygen/lib/http/codec/DefaultHTTPCodecFactory.h>
#include <proxygen/lib/http/session/HTTPDefaultSessionCodecFactory.h>

#include <proxygen/lib/http/codec/HTTP2Constants.h>

namespace proxygen {

HTTPDefaultSessionCodecFactory::HTTPDefaultSessionCodecFactory(
    const AcceptorConfiguration& accConfig)
    : accConfig_(accConfig) {
}

std::unique_ptr<HTTPCodec> HTTPDefaultSessionCodecFactory::getCodec(
    const std::string& nextProtocol, TransportDirection direction, bool isTLS) {
  DefaultHTTPCodecFactory factory(accConfig_.forceHTTP1_0_to_1_1,
                                  accConfig_.headerIndexingStrategy,
                                  accConfig_.allowedPlaintextUpgradeProtocols);
  factory.setStrictValidationFn([this] { return useStrictValidation(); });
  if (!isTLS &&
      (accConfig_.plaintextProtocol == http2::kProtocolCleartextString)) {
    return factory.getCodec(http2::kProtocolString, direction, isTLS);
  }
  return factory.getCodec(nextProtocol, direction, isTLS);
}
} // namespace proxygen
