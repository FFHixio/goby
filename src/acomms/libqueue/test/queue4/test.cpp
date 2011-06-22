// copyright 2011 t. schneider tes@mit.edu
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this software.  If not, see <http://www.gnu.org/licenses/>.

#include "test.pb.h"
#include "goby/acomms/queue.h"
#include "goby/acomms/connect.h"
#include "goby/util/binary.h"
#include "goby/util/logger.h"
#include "goby/acomms/libdccl/dccl_field_codec_default.h"

// tests multi-frame DCCL queuing with non-BROADCAST destination

using goby::acomms::operator<<;

int receive_count = 0;
bool handle_ack_called = false;
int goby_message_qsize = 0;
GobyMessage msg_in1, msg_in2;

void handle_ack(const goby::acomms::protobuf::ModemDataAck& ack_msg,
                const google::protobuf::Message& orig_msg);

void qsize(goby::acomms::protobuf::QueueSize size);

void handle_receive(const google::protobuf::Message &msg);

int main(int argc, char* argv[])
{    
    goby::glog.add_stream(goby::util::Logger::DEBUG3, &std::cerr);
    goby::glog.set_name(argv[0]);
    
    goby::acomms::QueueManager* q_manager = goby::acomms::QueueManager::get();
    goby::acomms::protobuf::QueueManagerConfig cfg;
    const int MY_MODEM_ID = 1;
    const int UNICORN_MODEM_ID = 3;
    cfg.set_modem_id(MY_MODEM_ID);
    q_manager->set_cfg(cfg);
    
    goby::acomms::DCCLModemIdConverterCodec::add("unicorn", UNICORN_MODEM_ID);
    goby::acomms::DCCLModemIdConverterCodec::add("topside", MY_MODEM_ID);
    
    goby::acomms::connect(&q_manager->signal_receive, &handle_receive);
    goby::acomms::connect(&q_manager->signal_queue_size_change, &qsize);
    goby::acomms::connect(&q_manager->signal_ack, &handle_ack);

    msg_in1.set_telegram("hello!");
    msg_in1.mutable_header()->set_time(
        goby::util::as<std::string>(boost::posix_time::second_clock::universal_time()));
    msg_in1.mutable_header()->set_source_platform("topside");
    msg_in1.mutable_header()->set_dest_platform("unicorn");
    msg_in1.mutable_header()->set_dest_type(Header::PUBLISH_OTHER);
    msg_in1.set_telegram("hello 1");
    
    msg_in2 = msg_in1;
    msg_in2.set_telegram("hello 2");
    
    std::cout << "Pushed: " << msg_in1 << std::endl;
    q_manager->push_message(msg_in1);
    std::cout << "Pushed: " << msg_in2 << std::endl;
    q_manager->push_message(msg_in2);


    goby::acomms::protobuf::ModemDataRequest request_msg;
    request_msg.set_max_bytes(16);
    request_msg.set_frame(0);
    request_msg.mutable_base()->set_dest(UNICORN_MODEM_ID);
    
    goby::acomms::protobuf::ModemDataTransmission data_msg_0, data_msg_1;
    q_manager->handle_modem_data_request(request_msg, &data_msg_0);

    request_msg.set_frame(1);
    q_manager->handle_modem_data_request(request_msg, &data_msg_1);
    

    std::cout << "requesting data for frame 0, got: " << data_msg_0 << std::endl;
    std::cout << "\tdata as hex: " << goby::util::hex_encode(data_msg_0.data()) << std::endl;
    std::cout << "requesting data for frame 1, got: " << data_msg_1 << std::endl;
    std::cout << "\tdata as hex: " << goby::util::hex_encode(data_msg_1.data()) << std::endl;

    // fake an ack from unicorn
    goby::acomms::protobuf::ModemDataAck ack;
    ack.mutable_base()->set_src(UNICORN_MODEM_ID);
    ack.mutable_base()->set_dest(MY_MODEM_ID);
    ack.set_frame(0);
    q_manager->handle_modem_ack(ack);
    assert(goby_message_qsize == 1);
    assert(handle_ack_called == true);

    handle_ack_called = false;
    
    ack.set_frame(1);
    q_manager->handle_modem_ack(ack);
    assert(goby_message_qsize == 0);
    assert(handle_ack_called == true);
    
    
    std::cout << "all tests passed" << std::endl;    
}

void handle_receive(const google::protobuf::Message &msg)
{
    std::cout << "Received: " << msg << std::endl;
    
    
    ++receive_count;
}

void qsize(goby::acomms::protobuf::QueueSize size)
{
    goby_message_qsize = size.size();
}


void handle_ack(const goby::acomms::protobuf::ModemDataAck& ack_msg,
                const google::protobuf::Message& orig_msg)
{
    std::cout << "got an ack: " << ack_msg << "\n" 
              << "of original: " << orig_msg << std::endl;
    handle_ack_called = true;
}