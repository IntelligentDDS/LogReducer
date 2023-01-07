package com.yuxiaoba.hello;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import java.util.*;

import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.Option;

public class Hello {
    public static void main(String[] args) {
        CommandLine line = parseArg(args);
        int logNumber = 100;
        String logLength = "log20";

        if (line.hasOption('h') == false) {
            if (line.hasOption("lognumber") == false) {
                System.out.println("Get empty lognumber, user 100 at default!");
            } else {
                logNumber = Integer.parseInt(line.getOptionValue("lognumber"));
            }
            if (line.hasOption("length") == false) {
                System.out.println("Get empty log length, user log20 at default!");
            } else {
                logLength = line.getOptionValue("length");   
            }
        }

        Hashtable<String, String> printLog = new Hashtable<String, String>();
        printLog.put("log20", "This is log hotspot!");
        printLog.put("log50", "This is hotspots.arXiEDHcPlfrcKwJxWjrjAYLvWkgzD");
        printLog.put("log100",
                "This is hotspots.bJzsxyrMpzYGcfgKquEUPTHQtiFzeEHJxTEmdAtQQHhpfjtrKClFLsfjacHuTDkgzWNGmSOaZnzLxoUp");
        printLog.put("log200",
                "This is hotspots.aRdUTDyLjPkZknRfLlCSxcEQLUKQTnbNCaMEnKLhVFmdNSQjInhGjrUMqMHPyScbMKBEPAIkxLukJUfImotUtzxIbhjMOHUpLqbbTYEXsOToClWMkOydioyhXzvcLiXtAMMssTROiXDwLLwInfaZyXptcxnRjPafxftKWtBBPQqodLPdvmEB");
        printLog.put("log500",
                "This is hotspots.kdeHKrCcqKeCxYqRbXDENnFBuJemiQMuNvresaULGgozUxAugbywwYJEaqQbYGWcISrRlEnXdpoNZNwvdnzxoaXLMWHLxghZixRDiZbVTKynrgQjJisDcaGwanDipFZGRLhJkXqnDlPlcaHBnYYwRQcpnbBWcsRwaOWyVjVORurodxkMGXvUPNoKJKcUrhiDuTpVCQWpmbJYkNYiVyiPYlllLCOMPIJGgLiUCjOSnmFcXtmnUXyKBvDgKTCVRLYZzkTPhtKPApvFMuVVuEcuqPcQLLbHHvumugBgmgOiguyYbdMsfwQLguqOBeakiSjhkSKBFTSjuoMViXPeMCQllfTdvPVJcRpjtACHfxJkhHYrLUyvCTKBvXzNHxikUFbngYdERraSrrvlGRciDalgxLIVTrbdEnORNIomHRtbFfPnJHFWLfAjFxWpKhLHZewgavRdVmbfSIzfHMfFxtIlEOpOyxGyAKnX");
        printLog.put("log1000",
                "This is hotspots.htTGLdjysnJBmraMbjsSytuKfxduwsdQJyzsIneGUNyiIYLwXBHgWupqOXLfUXOnpYbkJriscvFxGMRXbQsuipuBTsiMjpXKpbNcEoFWDDiMGeORcLWacLLOdcMaEVMcPDTpwNeEvUCgfcAARWsVBogiRstmyNCFwLfyvRdfdESZeSdjVzXDVGIKQztMylXzZfgCSzKIjiVtYwjHNTIwFDvhpyTgjKLfAFOHtJaVKEmzxDxrQJPmENyfBpQTDgFtwJYSwGPJkXBxoWKyhlVgavTnAOIgxSMsupviMzCttszFjubiNpFuierLCDVhsLudMgFbyaRFuapcYfGrvMVrIKWvoDEBHAdgLcGWVxMduuMkhHtAUtnCKHcYFkvXTCbixbGKpZBCuOkpcTCVTsGFZahRgzCwPHmKqaYougbHiOiDDSHatwIAKQDqhKNJEuydQfwcBoIeMotVHIpNzhcIndmUaNiHfYltuJuJNraDdUysIKwBNclJFLUbajVSgVheJbFdPjlTlnafBFAqeKQPvQvgjTLISfYKVmZcfReYOnRRlneMtBuVxCMapyYPZShEWkMsdTIYxLgjEawGXwXvSvUkAvclpOPKSTxrOIfHSMLcmRlfzXlbIPHpmncnfhGWIRfcVsZazehLGuYfVEfjlzuluPxlFKxeYJjmtEzYTVGOAjuMIpUYEwNeFNOeqoyHkJhrzNtsyiYBNHRmlMCZubXmwBRXingWgbYMRyabuABkGUpWQBQLdEtYHyRnCocXgirNamwpkBmdfjuzPzbvxtzdwHwXZrffSSeKPvkRkwLvofZmXBwABRhimRkAiMxFZRKflFdVKDcBjpCBvMZaJVnqyuqxCvfVRWzLijTQAjPIbBBRUTFKeRabpkeUddhfONWuRAhxbghoIKHIYhyvjBxyywIQsvUveuJvZzprbTaBKQJpRBiezwfTAtRDpmitJqPYxWhBhAuvvQjbbyjf");
        String logContent = printLog.get(logLength).toString();
        System.out.println("Start java log bench!");
        System.out.println(logNumber);
        Timer timer = new Timer();

        // run per 100ms, stop if run 1000 times
        timer.schedule(new Task(timer, logNumber, 10000,logContent), 0, 100);
    }

    private static CommandLine parseArg(String[] args) {
        // create the parser
        CommandLineParser parser = new DefaultParser();
        // create Options object
        final Options options = new Options();
        options.addOption(new Option("n", "lognumber", true, "Print number of log per 100ms"));
        options.addOption(new Option("l", "length", true, "Print the lenghth of log"));
        options.addOption(new Option("h", "help", false, "Display help text."));

        try {
            // parse the command line arguments
            CommandLine line = parser.parse(options, args);

            if (line.hasOption('h')) {
                // automatically generate the help statement
                HelpFormatter formatter = new HelpFormatter();
                formatter.printHelp("java log bench help", options);
            }
            return line;
        } catch (ParseException exp) {
            // oops, something went wrong
            System.err.println("Parsing failed.  Reason: " + exp.getMessage());

        }
        return null;
    }

}

class Task extends TimerTask {
    private Timer timer;
    private int exeCount;
    private int logNumber;
    private String logContent;
    private static final Logger logger = LogManager.getLogger(Hello.class.getName());

    public Task(Timer timer, int logNumber, int exeCount, String logContent) {
        this.timer = timer;
        this.exeCount = exeCount;
        this.logNumber = logNumber;
        this.logContent = logContent;
    }

    private int count = 1;

    @Override
    public void run() {
        logger.error("This is not log hotspots");

        for (int i = 0; i < logNumber; i++) {
            logger.error(logContent);
        }
        count = count + 1;

        // 若满足此条件 退出此线程
        if (count > exeCount) {
            this.timer.cancel();
            System.out.println("Java log bench stop");
        }
    }
}