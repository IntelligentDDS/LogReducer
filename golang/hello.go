package main

import (
	"flag"
	"fmt"
	"os"
	"time"

	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"
)

var sugarLogger *zap.SugaredLogger

func InitLogger() {
	writeSyncer := getLogWriter()
	encoder := getEncoder()
	core := zapcore.NewCore(encoder, writeSyncer, zapcore.DebugLevel)

	logger := zap.New(core, zap.AddCaller())
	sugarLogger = logger.Sugar()
}

func getEncoder() zapcore.Encoder {
	encoderConfig := zapcore.EncoderConfig{
		TimeKey:        "ts",
		LevelKey:       "level",
		NameKey:        "logger",
		CallerKey:      "caller",
		FunctionKey:    zapcore.OmitKey,
		MessageKey:     "msg",
		StacktraceKey:  "stacktrace",
		LineEnding:     zapcore.DefaultLineEnding,
		EncodeLevel:    zapcore.LowercaseLevelEncoder,
		EncodeTime:     zapcore.TimeEncoderOfLayout("2006-01-02 15:04:05.000"),
		EncodeDuration: zapcore.SecondsDurationEncoder,
		EncodeCaller:   zapcore.ShortCallerEncoder,
	}
	return zapcore.NewJSONEncoder(encoderConfig)
}

func getLogWriter() zapcore.WriteSyncer {
	file, _ := os.Create("/data/home/logbench/golang/golang.log")
	return zapcore.AddSync(file)
}

var (
	h         bool
	logNumber int
	logLength string
)

func init() {
	const (
		loglenth = "log20"
	)

	flag.BoolVar(&h, "h", false, "this help")
	flag.StringVar(&logLength, "length", loglenth, "log length")
	flag.IntVar(&logNumber, "lognumber", 100, "log number")
}

func main() {
	// _, err := os.Stat("golang.log")
	// fmt.Println(err)
	// if err != nil {
	// 	fmt.Println(err)
	// } else {
	// 	err = os.Remove("/data/home/bobgbyu/LangBench/golang/golang.log")
	// 	if err != nil {
	// 		fmt.Println(err)
	// 	}
	// }

	InitLogger()
	defer sugarLogger.Sync()

	printLog := map[string]string{"log20": "This is log hotspot!", "log50": "This is hotspots.arXiEDHcPlfrcKwJxWjrjAYLvWkgzD", "log100": "This is hotspots.bJzsxyrMpzYGcfgKquEUPTHQtiFzeEHJxTEmdAtQQHhpfjtrKClFLsfjacHuTDkgzWNGmSOaZnzLxoUp",
		"log200":  "This is hotspots.aRdUTDyLjPkZknRfLlCSxcEQLUKQTnbNCaMEnKLhVFmdNSQjInhGjrUMqMHPyScbMKBEPAIkxLukJUfImotUtzxIbhjMOHUpLqbbTYEXsOToClWMkOydioyhXzvcLiXtAMMssTROiXDwLLwInfaZyXptcxnRjPafxftKWtBBPQqodLPdvmEB",
		"log500":  "This is hotspots.kdeHKrCcqKeCxYqRbXDENnFBuJemiQMuNvresaULGgozUxAugbywwYJEaqQbYGWcISrRlEnXdpoNZNwvdnzxoaXLMWHLxghZixRDiZbVTKynrgQjJisDcaGwanDipFZGRLhJkXqnDlPlcaHBnYYwRQcpnbBWcsRwaOWyVjVORurodxkMGXvUPNoKJKcUrhiDuTpVCQWpmbJYkNYiVyiPYlllLCOMPIJGgLiUCjOSnmFcXtmnUXyKBvDgKTCVRLYZzkTPhtKPApvFMuVVuEcuqPcQLLbHHvumugBgmgOiguyYbdMsfwQLguqOBeakiSjhkSKBFTSjuoMViXPeMCQllfTdvPVJcRpjtACHfxJkhHYrLUyvCTKBvXzNHxikUFbngYdERraSrrvlGRciDalgxLIVTrbdEnORNIomHRtbFfPnJHFWLfAjFxWpKhLHZewgavRdVmbfSIzfHMfFxtIlEOpOyxGyAKnX",
		"log1000": "This is hotspots.htTGLdjysnJBmraMbjsSytuKfxduwsdQJyzsIneGUNyiIYLwXBHgWupqOXLfUXOnpYbkJriscvFxGMRXbQsuipuBTsiMjpXKpbNcEoFWDDiMGeORcLWacLLOdcMaEVMcPDTpwNeEvUCgfcAARWsVBogiRstmyNCFwLfyvRdfdESZeSdjVzXDVGIKQztMylXzZfgCSzKIjiVtYwjHNTIwFDvhpyTgjKLfAFOHtJaVKEmzxDxrQJPmENyfBpQTDgFtwJYSwGPJkXBxoWKyhlVgavTnAOIgxSMsupviMzCttszFjubiNpFuierLCDVhsLudMgFbyaRFuapcYfGrvMVrIKWvoDEBHAdgLcGWVxMduuMkhHtAUtnCKHcYFkvXTCbixbGKpZBCuOkpcTCVTsGFZahRgzCwPHmKqaYougbHiOiDDSHatwIAKQDqhKNJEuydQfwcBoIeMotVHIpNzhcIndmUaNiHfYltuJuJNraDdUysIKwBNclJFLUbajVSgVheJbFdPjlTlnafBFAqeKQPvQvgjTLISfYKVmZcfReYOnRRlneMtBuVxCMapyYPZShEWkMsdTIYxLgjEawGXwXvSvUkAvclpOPKSTxrOIfHSMLcmRlfzXlbIPHpmncnfhGWIRfcVsZazehLGuYfVEfjlzuluPxlFKxeYJjmtEzYTVGOAjuMIpUYEwNeFNOeqoyHkJhrzNtsyiYBNHRmlMCZubXmwBRXingWgbYMRyabuABkGUpWQBQLdEtYHyRnCocXgirNamwpkBmdfjuzPzbvxtzdwHwXZrffSSeKPvkRkwLvofZmXBwABRhimRkAiMxFZRKflFdVKDcBjpCBvMZaJVnqyuqxCvfVRWzLijTQAjPIbBBRUTFKeRabpkeUddhfONWuRAhxbghoIKHIYhyvjBxyywIQsvUveuJvZzprbTaBKQJpRBiezwfTAtRDpmitJqPYxWhBhAuvvQjbbyjf"}

	flag.Parse()

	logContent := printLog[logLength]

	startT := time.Now() //计算当前时间
	// log_number := 1000   // print time per 100 ms
	interval := time.Duration(100)
	t := time.NewTicker(interval * time.Millisecond)

	count := 0
	cycle := 10000
	for {
		<-t.C
		count++
		sugarLogger.Error("This is not hotspots")
		for i := 0; i < logNumber; i++ {
			sugarLogger.Error(logContent)
		}
		if count > cycle {
			t.Stop()
			tc := time.Since(startT) //计算耗时
			fmt.Printf("time cost = %v\n", tc)
		}
	}

	// for cycle := 1000; cycle > 0; cycle++ {
	// 	sugarLogger.Error("This is not hotspots")
	// 	start := time.Now().UTC()
	// 	for i := 0; i < each_interval_log; i++ {
	// 		sugarLogger.Error("This is not hotspots", i)
	// 	}
	// 	pTime := time.Now().UTC().Sub(start)
	// 	var pSecond = float64(pTime)
	// 	fmt.Println(time.Second*time.Duration(interval) - time.Nanosecond*time.Duration(pSecond))
	// 	time.Sleep(time.Second*time.Duration(interval) - time.Nanosecond*time.Duration(pSecond))
	// }

	// for i := 0; i < 1000; i++ {
	// 	for j := 0; j < 10000; j++ {
	// 		sugarLogger.Error(i, j)
	// 	}
	// }

}
