/*
 * Copyright (c) 2019, NVIDIA CORPORATION. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of NVIDIA CORPORATION nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <nvidia/aiaa/client.h>
#include "../commonutils.h"

int main(int argc, char **argv) {
  if (argc < 2 || cmdOptionExists(argv, argv + argc, "-h")) {
    std::cout << "Usage:: <COMMAND> <OPTIONS>\n"
        "  |-h        (Help) Print this information                                                |\n"
        "  |-server   Server URI  {default: http://10.110.45.66:5000/v1}                           |\n"
        "  |-ratio    Point Ratio {default: 10}                                                    |\n"
        " *|-image    Input Image File                                                             |\n"
        "  |-format   Format Output Json                                                           |\n"
        "  |-output   Output File Name to store result                                             |\n";
    return 0;
  }

  std::string serverUri = getCmdOption(argv, argv + argc, "-server", "http://10.110.45.66:5000/v1");
  int ratio = ::atoi(getCmdOption(argv, argv + argc, "-ratio", "10").c_str());
  std::string inputImageFile = getCmdOption(argv, argv + argc, "-image");
  std::string outputJsonFile = getCmdOption(argv, argv + argc, "-output");
  int jsonSpace = cmdOptionExists(argv, argv + argc, "-format") ? 2 : 0;

  if (ratio < 1) {
    std::cerr << "Invalid Point Ratio (should be > 0)\n";
    return -1;
  }
  if (inputImageFile.empty()) {
    std::cerr << "Input Image file is missing\n";
    return -1;
  }

  try {
    nvidia::aiaa::Client client(serverUri);
    nvidia::aiaa::PolygonsList result = client.mask2Polygon(ratio, inputImageFile);

    if (outputJsonFile.empty()) {
      std::cout << result.toJson(jsonSpace) << std::endl;
    } else {
      stringToFile(result.toJson(jsonSpace), outputJsonFile);
    }
    return 0;
  } catch (nvidia::aiaa::exception& e) {
    std::cerr << "nvidia::aiaa::exception => nvidia.aiaa.error." << e.id << "; description: " << e.name() << std::endl;
  }

  return -1;
}