/*******************************************************************************
 * Copyright (c) 2012-2014, The Microsystems Design Labratory (MDL)
 * Department of Computer Science and Engineering, The Pennsylvania State
 *University All rights reserved.
 *
 * This source code is part of NVMain - A cycle accurate timing, bit accurate
 * energy simulator for both volatile (e.g., DRAM) and non-volatile memory
 * (e.g., PCRAM). The source code is free and you can redistribute and/or
 * modify it by providing that the following conditions are met:
 *
 *  1) Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2) Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author list:
 *   Matt Poremba    ( Email: mrp5060 at psu dot edu
 *                     Website: http://www.cse.psu.edu/~poremba/ )
 *******************************************************************************/

#ifdef TU_DORTMUND
#include "DefaultTraceWriter.hh"
#include <cstdint>
#include <iostream>
#include <string>

namespace gem5 {

DefaultTraceWriter::DefaultTraceWriter() {}

DefaultTraceWriter::~DefaultTraceWriter() {
    std::cout << "Closing the trace, writing out results" << std::endl;
    WriteTraceLine(trace);
}
void DefaultTraceWriter::SetTraceFile(std::string file) {
    // Note: This function assumes an absolute path is given, otherwise
    // the current directory is used.

    traceFile = file;

    trace.open(traceFile.c_str());

    if (!trace.is_open()) {
        std::cout << "Warning: Could not open trace file " << file
                  << ". Output will be suppressed." << std::endl;
    }

    /* Write version number of this writer. */
    // trace << "NVMV1" << std::endl;
}

std::string DefaultTraceWriter::GetTraceFile() { return traceFile; }

bool DefaultTraceWriter::SetNextAccess(memory_content nextAccess) {
    bool rv = false;
    if(nextAccess.getAddress()) {
        traceLines.push_back(nextAccess);
        rv = true;
    }
    return rv;
}

void DefaultTraceWriter::WriteTraceLine(std::ostream& stream) {

    for(memory_content line : traceLines) {
        if(line.getAddress()) {

            /* Print memory cycle. */
            stream << line.getTick() << " ";

            /* Print the operation type */
            if (line.getCmd())
                stream << "W ";
            else
                stream << "R ";

            /* Print address */
            stream << std::hex << "0x" << line.getAddress()
                << std::dec << " ";

            /* Print data. */
            stream << (int) *line.getNewContent() << " ";

            /* Print previous data. */
            stream << (int) *line.getOldContent() << " ";

            /* Print the BitFlips */
            stream << bitFlipsToString(line.getBitFlips()) << std::endl;
            
        }

        else 
        {
            stream << "nullptr";
        }
    }
}

std::string DefaultTraceWriter::bitFlipsToString(uint64_t* array) {
        std::string result;
        for (int i = 0; i < 64; i++) {
            if(i==0)
                result += "[" + std::to_string(array[i]) + ", ";
            else if(i<63 && i>0 && i%7==0)
                result += std::to_string(array[i]) + " | ";
            else if(i<63 && i>0 && i%7!=0)
                result += std::to_string(array[i]) + ", ";
            else if (i==63) {
                result += std::to_string(array[i]) + "]";
            }
        }
        return result;
    }

}; //namespace gem5
#endif