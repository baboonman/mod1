#include "Task.hpp"

cl_kernel       Task::getKernel() {
    return this->_kernel;
}

std::string     *Task::getSrc(std::string filename) const {
    std::ifstream ifs(filename);
    std::string *content;
        
    content = new std::string( (std::istreambuf_iterator<char>(ifs) ),
                            (std::istreambuf_iterator<char>()) );
    return content;
}

void            Task::enqueueKernel(cl_command_queue queue) {
    int     err;

    err = clEnqueueNDRangeKernel(
            queue,
            this->_kernel,
            1,
            NULL,
            &this->_globalWorkSize,
            &this->_localWorkSize,
            0,
            NULL,
            NULL);
    checkCLSuccess(err, "clEnqueueNDRangeKernel");
}
