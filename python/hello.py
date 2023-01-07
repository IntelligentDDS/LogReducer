import datetime
from nb_log import get_logger
import timeit
import time
import argparse
import os

os.remove("/data/home/logbench/python/python.log")

# logger = get_logger('python',, log_filename='python.log',formatter_template=5,log_file_handler_type=3,is_add_stream_handler=False)

print_log = {"log20": "This is log hotspot!", "log50": "This is hotspots.arXiEDHcPlfrcKwJxWjrjAYLvWkgzD", "log100": "This is hotspots.bJzsxyrMpzYGcfgKquEUPTHQtiFzeEHJxTEmdAtQQHhpfjtrKClFLsfjacHuTDkgzWNGmSOaZnzLxoUp",
             "log200": "This is hotspots.aRdUTDyLjPkZknRfLlCSxcEQLUKQTnbNCaMEnKLhVFmdNSQjInhGjrUMqMHPyScbMKBEPAIkxLukJUfImotUtzxIbhjMOHUpLqbbTYEXsOToClWMkOydioyhXzvcLiXtAMMssTROiXDwLLwInfaZyXptcxnRjPafxftKWtBBPQqodLPdvmEB",
             "log500": "This is hotspots.kdeHKrCcqKeCxYqRbXDENnFBuJemiQMuNvresaULGgozUxAugbywwYJEaqQbYGWcISrRlEnXdpoNZNwvdnzxoaXLMWHLxghZixRDiZbVTKynrgQjJisDcaGwanDipFZGRLhJkXqnDlPlcaHBnYYwRQcpnbBWcsRwaOWyVjVORurodxkMGXvUPNoKJKcUrhiDuTpVCQWpmbJYkNYiVyiPYlllLCOMPIJGgLiUCjOSnmFcXtmnUXyKBvDgKTCVRLYZzkTPhtKPApvFMuVVuEcuqPcQLLbHHvumugBgmgOiguyYbdMsfwQLguqOBeakiSjhkSKBFTSjuoMViXPeMCQllfTdvPVJcRpjtACHfxJkhHYrLUyvCTKBvXzNHxikUFbngYdERraSrrvlGRciDalgxLIVTrbdEnORNIomHRtbFfPnJHFWLfAjFxWpKhLHZewgavRdVmbfSIzfHMfFxtIlEOpOyxGyAKnX",
             "log1000": "This is hotspots.htTGLdjysnJBmraMbjsSytuKfxduwsdQJyzsIneGUNyiIYLwXBHgWupqOXLfUXOnpYbkJriscvFxGMRXbQsuipuBTsiMjpXKpbNcEoFWDDiMGeORcLWacLLOdcMaEVMcPDTpwNeEvUCgfcAARWsVBogiRstmyNCFwLfyvRdfdESZeSdjVzXDVGIKQztMylXzZfgCSzKIjiVtYwjHNTIwFDvhpyTgjKLfAFOHtJaVKEmzxDxrQJPmENyfBpQTDgFtwJYSwGPJkXBxoWKyhlVgavTnAOIgxSMsupviMzCttszFjubiNpFuierLCDVhsLudMgFbyaRFuapcYfGrvMVrIKWvoDEBHAdgLcGWVxMduuMkhHtAUtnCKHcYFkvXTCbixbGKpZBCuOkpcTCVTsGFZahRgzCwPHmKqaYougbHiOiDDSHatwIAKQDqhKNJEuydQfwcBoIeMotVHIpNzhcIndmUaNiHfYltuJuJNraDdUysIKwBNclJFLUbajVSgVheJbFdPjlTlnafBFAqeKQPvQvgjTLISfYKVmZcfReYOnRRlneMtBuVxCMapyYPZShEWkMsdTIYxLgjEawGXwXvSvUkAvclpOPKSTxrOIfHSMLcmRlfzXlbIPHpmncnfhGWIRfcVsZazehLGuYfVEfjlzuluPxlFKxeYJjmtEzYTVGOAjuMIpUYEwNeFNOeqoyHkJhrzNtsyiYBNHRmlMCZubXmwBRXingWgbYMRyabuABkGUpWQBQLdEtYHyRnCocXgirNamwpkBmdfjuzPzbvxtzdwHwXZrffSSeKPvkRkwLvofZmXBwABRhimRkAiMxFZRKflFdVKDcBjpCBvMZaJVnqyuqxCvfVRWzLijTQAjPIbBBRUTFKeRabpkeUddhfONWuRAhxbghoIKHIYhyvjBxyywIQsvUveuJvZzprbTaBKQJpRBiezwfTAtRDpmitJqPYxWhBhAuvvQjbbyjf"}

logger = get_logger(name="pythonbench", is_add_stream_handler=False,
                    log_path="/data/home/logbench/python",  log_filename='python.log', log_file_handler_type=3)

if __name__ == "__main__":
    # number = 10000  # 试验次数
    # msg = "The benchmark cannot handler so mush logs at the intervarl, please reduce lognumber"  # 日志内容
    # time_pass = timeit.timeit('logger.info(msg)', setup='from __main__ import logger, msg', number=number)
    # rate = number / time_pass  # 每秒最大次数
    # print(rate)
    parser = argparse.ArgumentParser(
        description="generate specific number of logs per second",
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--lognumber", type=int, default="100",
                        help="number of logs per 100ms (less than 28000 in python)")
    parser.add_argument("--length", type=str, default="log20",
                        help="length of log")
    args = parser.parse_args()
    

    interval = 0.1
    log_number = args.lognumber
    cycle = 10000
    each_interval_log = int(log_number)

    start = datetime.datetime.now()
    while cycle > 0:
        logger.error("This is not log hotspot")
        begin = time.time()
        for i in range(each_interval_log):
            logger.error("%s", print_log[args.length])
        if interval-float(time.time()-begin) > 0:
            # print(interval-float(time.time()-begin))
            time.sleep(interval-float(time.time()-begin))
        else:
            logger.error(
                "The benchmark cannot handler so mush logs at the intervarl, please reduce lognumber")

        cycle = cycle - 1

    print((datetime.datetime.now()-start))
    print("Python bench finish")
