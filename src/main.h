#pragma once

#include "gargamel/gargamel.h"

/**
 * Command line option names.
 *
 * For use with gargamel.
 * \sa gargamel.h
 */
enum ArgNames {
	Unknown,
	Help,
	LogFile,
	EchoLog,
	LogChannel,
	LogAll,
	ScreenWidth,
	ScreenHeight,
	FullScreen,
	RunUnitTests,
};

//Command line option descriptions
START_ARGS(Arguments)
	DESCRIBE_ARG(Unknown,
		'\0',
		nullptr,
		NoArg,
		"Usage Instructions:\n")
	DESCRIBE_ARG(Help,
		'h',
		"help",
		NoArg,
		"\tPrint help text\n")
	DESCRIBE_ARG(LogFile,
		'\0',
		"log",
		RequiredArg,
		"\tSpecify log file\n")
	DESCRIBE_ARG(EchoLog,
		'e',
		"echo",
		NoArg,
		"\tEcho log to console\n")
	DESCRIBE_ARG(LogAll,
		'v',
		"verbose",
		NoArg,
		"\tLog all messages, regardless of channel\n")
	DESCRIBE_ARG_ARRAY(LogChannel,
		"log-channel",
		"\tTurn on logging channel\n")
	DESCRIBE_ARG_DEFAULT(ScreenWidth,
		'\0',
		"width",
		RequiredArg,
		"800",
		"\tSet the width of the window\n")
	DESCRIBE_ARG_DEFAULT(ScreenHeight,
		'\0',
		"height",
		RequiredArg,
		"600",
		"\tSet the height of the window\n")
	DESCRIBE_ARG(FullScreen,
		'f',
		"full-screen",
		NoArg,
		"\tStart in full screen mode\n")
	DESCRIBE_ARG(RunUnitTests,
		't',
		"run-unit-tests",
		NoArg,
		"\tRuns any unit tests the game has defined, then exits\n")
END_ARGS

void Init(int argc, char* argv[]);
int main(int argc, char* argv[]);
