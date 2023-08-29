#include "TraceEventObject.hh"
#include "params/SimObject.hh"
#include "params/TraceEventObject.hh"
#include "sim/cur_tick.hh"
#include <cstddef>
#include <ios>
#include <iostream>
#include <iterator>



namespace gem5
{

TraceEventObject::TraceEventObject(const TraceEventObjectParams &params) :
    SimObject(params),
    event([this]{processEvent();}, 
    name()), 
    time(params.timer)

{
    std::cout << "Created TraceObject";
}

int
TraceEventObject::getIndex()
{
    return index;
}

bool
TraceEventObject::getFlag() 
{
    return this->flag;
}

void
TraceEventObject::setFlag()
{
    this->flag = 0;
}

void 
TraceEventObject::resetFile()
{
    trace.open(filePath);
    trace.close();
}

void 
TraceEventObject::processEvent()
{
    trace.open(filePath, std::ios::out | std::ios::app);
    if (buffer.size() > 0 && getIndex() < buffer.size()) {
        tracer->WriteTraceLine(trace, buffer.at(getIndex()));
        addIndex();
    }
    trace.close();
    schedule(event, curTick() + time);
}

void
TraceEventObject::startup()
{
    buffer.clear();
    resetFile();
    schedule(event, curTick() + time);
}

void
TraceEventObject::addIndex()
{
    index++;
}

void
TraceEventObject::insertBuffer(memory_content c)
{   
    buffer.push_back(c);
}

} //namespace gem5