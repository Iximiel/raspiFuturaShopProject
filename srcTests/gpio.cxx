#include <iostream>
#include <string>
#include <gpiod.hpp>
#include <chrono>
#include <thread>

void print_event(const ::gpiod::line_event& event)
{
  if (event.event_type == ::gpiod::line_event::RISING_EDGE) {
    std::cout << "\nRISING EDGE";
  } else if (event.event_type == ::gpiod::line_event::FALLING_EDGE) {
    std::cout << "\nFALLING EDGE";
  } else {
    throw ::std::logic_error("invalid event type");
  }

  std::cout << " ";

  std::cout << ::std::chrono::duration_cast<::std::chrono::seconds>(event.timestamp).count();
  std::cout << ".";
  std::cout << event.timestamp.count() % 1000000000;

  std::cout << " line: " << event.source.offset();

  std::cout << ::std::endl;
}


int main (int, char**argv) {
  int retval =0;
  try{
    gpiod::chip GPIO("pinctrl-bcm2835");
    /*for( int i =0; i<GPIO.num_lines();++i) {
      std::cout << i
      << '\t' << GPIO.get_line(i).name()
      << '\t' << GPIO.get_line(i).direction()
      << '\t' << GPIO.get_line(i).is_used()
      << std::endl;
      }*/
    /*
      GPIO.get_line(24).set_value(1);
      std::this_thread::sleep_for (std::chrono::seconds(1));
      GPIO.get_line(24).set_value(0);
    */
    auto line = GPIO.get_line(24);
    if (!line.is_requested()){      
      line.request({
		    argv[0],
		    ::gpiod::line_request::DIRECTION_OUTPUT,
		    0
	}, 1);
      //      line.set_value(0);
      std::this_thread::sleep_for (std::chrono::seconds(1));
      line.set_value(0);
      /*
	line.request({
	argv[0],
	::gpiod::line_request::DIRECTION_OUTPUT,
	0
	}, 0);*/
      bool wait = true;
      std::this_thread::sleep_for (std::chrono::seconds(1));
      auto button = GPIO.get_line(22);
      if (!button.is_requested()){      
	button.request({
			argv[0],
			//gpiod::line_request::EVENT_BOTH_EDGES,
			gpiod::line_request::EVENT_RISING_EDGE,
			0
	  }, 0);
      }
      int i =0;
      while (wait) {
	bool event = button.event_wait(::std::chrono::seconds(1));
	if (event) {
	  ++i;
	  int val=line.get_value();
	  if (val==0) {
	    line.set_value(1);
	  } else {
	    line.set_value(0);
	  }
	  
	  print_event(button.event_read());
	  if (i>5){
	    wait=false;
	  }
	} else {
	  std::cout << '.' << std::flush;
	}
      }
    
    }
  } catch (const char *problem) {
    std::cerr << "\033[1;31mERROR: \033[0m\033[1m" << problem << "\033[0m" << std::endl;
    retval = 1;
  } catch (const std::string &problem) {
    std::cerr << "\033[1;31mERROR: \033[0m\033[1m" << problem << "\033[0m" << std::endl;
    retval = 1;
  }
  return retval;
}
