#include <iomanip>
#include <unistd.h>
#include "log4cplus/logger.h"
#include "log4cplus/fileappender.h"
#include "log4cplus/loglevel.h"
#include "log4cplus/loggingmacros.h"
#include "log4cplus/initializer.h"
#include <argparse/argparse.hpp>

using namespace std;
using namespace log4cplus;
using namespace chrono;

int main(int argc, char *argv[])
{   
    argparse::ArgumentParser program("hello");
    program.add_argument("-lognumber")
        .help("log number")
        .default_value(100)
        .scan<'i', int>();
    program.add_argument("-length")
    .default_value(std::string{"log20"})   // might otherwise be type const char* leading to an error when trying program.get<std::string>
    .help("log length");

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    int log_number = program.get<int>("-lognumber");    
    auto log_length = program.get<std::string>("-length");  
     
    map<string, string> dict{{"log20", "This is log hotspot!"}, {"log50", "This is hotspots.arXiEDHcPlfrcKwJxWjrjAYLvWkgzD"}, {"log100", "This is hotspots.bJzsxyrMpzYGcfgKquEUPTHQtiFzeEHJxTEmdAtQQHhpfjtrKClFLsfjacHuTDkgzWNGmSOaZnzLxoUp"},
             {"log200", "This is hotspots.aRdUTDyLjPkZknRfLlCSxcEQLUKQTnbNCaMEnKLhVFmdNSQjInhGjrUMqMHPyScbMKBEPAIkxLukJUfImotUtzxIbhjMOHUpLqbbTYEXsOToClWMkOydioyhXzvcLiXtAMMssTROiXDwLLwInfaZyXptcxnRjPafxftKWtBBPQqodLPdvmEB"},
             {"log500", "This is hotspots.kdeHKrCcqKeCxYqRbXDENnFBuJemiQMuNvresaULGgozUxAugbywwYJEaqQbYGWcISrRlEnXdpoNZNwvdnzxoaXLMWHLxghZixRDiZbVTKynrgQjJisDcaGwanDipFZGRLhJkXqnDlPlcaHBnYYwRQcpnbBWcsRwaOWyVjVORurodxkMGXvUPNoKJKcUrhiDuTpVCQWpmbJYkNYiVyiPYlllLCOMPIJGgLiUCjOSnmFcXtmnUXyKBvDgKTCVRLYZzkTPhtKPApvFMuVVuEcuqPcQLLbHHvumugBgmgOiguyYbdMsfwQLguqOBeakiSjhkSKBFTSjuoMViXPeMCQllfTdvPVJcRpjtACHfxJkhHYrLUyvCTKBvXzNHxikUFbngYdERraSrrvlGRciDalgxLIVTrbdEnORNIomHRtbFfPnJHFWLfAjFxWpKhLHZewgavRdVmbfSIzfHMfFxtIlEOpOyxGyAKnX"},
             {"log1000", "This is hotspots.htTGLdjysnJBmraMbjsSytuKfxduwsdQJyzsIneGUNyiIYLwXBHgWupqOXLfUXOnpYbkJriscvFxGMRXbQsuipuBTsiMjpXKpbNcEoFWDDiMGeORcLWacLLOdcMaEVMcPDTpwNeEvUCgfcAARWsVBogiRstmyNCFwLfyvRdfdESZeSdjVzXDVGIKQztMylXzZfgCSzKIjiVtYwjHNTIwFDvhpyTgjKLfAFOHtJaVKEmzxDxrQJPmENyfBpQTDgFtwJYSwGPJkXBxoWKyhlVgavTnAOIgxSMsupviMzCttszFjubiNpFuierLCDVhsLudMgFbyaRFuapcYfGrvMVrIKWvoDEBHAdgLcGWVxMduuMkhHtAUtnCKHcYFkvXTCbixbGKpZBCuOkpcTCVTsGFZahRgzCwPHmKqaYougbHiOiDDSHatwIAKQDqhKNJEuydQfwcBoIeMotVHIpNzhcIndmUaNiHfYltuJuJNraDdUysIKwBNclJFLUbajVSgVheJbFdPjlTlnafBFAqeKQPvQvgjTLISfYKVmZcfReYOnRRlneMtBuVxCMapyYPZShEWkMsdTIYxLgjEawGXwXvSvUkAvclpOPKSTxrOIfHSMLcmRlfzXlbIPHpmncnfhGWIRfcVsZazehLGuYfVEfjlzuluPxlFKxeYJjmtEzYTVGOAjuMIpUYEwNeFNOeqoyHkJhrzNtsyiYBNHRmlMCZubXmwBRXingWgbYMRyabuABkGUpWQBQLdEtYHyRnCocXgirNamwpkBmdfjuzPzbvxtzdwHwXZrffSSeKPvkRkwLvofZmXBwABRhimRkAiMxFZRKflFdVKDcBjpCBvMZaJVnqyuqxCvfVRWzLijTQAjPIbBBRUTFKeRabpkeUddhfONWuRAhxbghoIKHIYhyvjBxyywIQsvUveuJvZzprbTaBKQJpRBiezwfTAtRDpmitJqPYxWhBhAuvvQjbbyjf"}};

    log4cplus::Initializer initializer;
    SharedAppenderPtr append_1(new FileAppender("c++.log"));
    append_1->setName(LOG4CPLUS_TEXT("append_1"));
    log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%Y/%m/%d %H:%M:%S,%Q} %-5p - [%l] %m %n");
    append_1->setLayout(std::unique_ptr<Layout>(new PatternLayout(pattern)));
    Logger logger_1 = Logger::getInstance(LOG4CPLUS_TEXT("c++"));
    logger_1.addAppender(append_1);
    logger_1.setLogLevel(log4cplus::WARN_LOG_LEVEL);

    int cycle = 10000;

    while (cycle > 0)
    {
        cycle = cycle - 1;
        LOG4CPLUS_ERROR(logger_1, LOG4CPLUS_TEXT("This is not hotspots"));
        auto start = system_clock::now();
        for( int a = 0; a < log_number; a = a + 1 )
        {
            LOG4CPLUS_ERROR(logger_1, "" << dict[log_length]);
        }
        auto end   = system_clock::now();
        int duration = duration_cast<microseconds>(end - start).count();
        // LOG4CPLUS_ERROR(logger_1, "Runtime: " << duration);
        int sleep = 100000 - duration;  // 100ms - duration
        usleep(sleep); // sleep us
    }

    return 0;
}
 