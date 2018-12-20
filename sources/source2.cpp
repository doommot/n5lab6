// Copyright 2018 Your Name <your_email>

#define BOOST_LOG_DYN_LINK 1

#include <thread>
#include <mutex>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <cstdio>

#include "picosha2.h"

// #include <boost/log/core.hpp>
// #include <boost/log/trivial.hpp>
// #include <boost/log/expressions.hpp>
// //#include <boost/log/sinks/text_file_backend.hpp>
// //#include <boost/log/utility/setup/file.hpp>
// //#include <boost/log/utility/setup/common_attributes.hpp>
// //#include <boost/log/sources/severity_logger.hpp>
// //#include <boost/log/sources/record_ostream.hpp>

#define difficulty 4 //задаёт сложность хеширования в полубайтах
#define hex_seed_length 64
#define log_rotation_size 10 // размер ротации лога в Мб
#define duration 10000 //длительность хеширования одного потока в количестве рассчитанных значений
// 0 для бесконечного поиска решений.

using namespace std;
// namespace logging = boost::log;

mutex rand_mutex;

void hash_thread();
void init(int argc, char *argv[], unsigned int *thread_amount);
string random_string();
bool check_hash(string hash);

int main(int argc, char *argv[])
{
	unsigned int thread_amount;
	init(argc, argv, &thread_amount);

	thread hash_thread_array[thread_amount];
	for (int i = 0; i<thread_amount; i++)
		hash_thread_array[i] = thread(hash_thread);

	for (int i = 0; i<thread_amount; i++)
		hash_thread_array[i].join();
	return 0;
}

void init(int argc, char *argv[], unsigned int *thread_amount)
{

	// logging::core::get()->set_filter
	// (
	// 	logging::trivial::severity >= logging::trivial::debug // filter to output only significant records
	// );

	// logging::add_file_log(logging::keywords::file_name = "log_%N.log", logging::keywords::rotation_size = log_rotation_size * 1024 * 1024);

	//unsigned int hardware_concurrency = thread::hardware_concurrency;//debug

	if(argc>1)
		if(stoi(argv[1])>thread::hardware_concurrency())
		{
			// BOOST_LOG_TRIVIAL(error) << "Hardware can provide " << thread::hardware_concurrency() << " threads, but " << argv[1] << " were given.";
			*thread_amount = thread::hardware_concurrency();
		}
		else *thread_amount = stoi(argv[1]);
	else *thread_amount = thread::hardware_concurrency();
	// BOOST_LOG_TRIVIAL(debug) << "thread_amount is set to " << *thread_amount;
}

void hash_thread()
{
	// BOOST_LOG_TRIVIAL(trace) << "thread " << this_thread::get_id() << " started";
	
	string seed;
	string hash;
	
	if(!duration)
		while(1)
		{
			seed = random_string();
			// BOOST_LOG_TRIVIAL(trace) << "thread " << this_thread::get_id() << " gets random seed "<< seed;
			hash = picosha2::hash256_hex_string(seed);
			cout << "thread " << this_thread::get_id() << " gets random seed "<< seed << "\n";
			// BOOST_LOG_TRIVIAL(trace) << "thread " << this_thread::get_id() << " gets hash "<< hash;
			cout << "thread " << this_thread::get_id() << " gets hash "<< hash << "\n";

			if(check_hash(hash))
			{
				// BOOST_LOG_TRIVIAL(info) << "valid hash found " << hash;
				cout << "valid hash found! " << hash << "\n";
				rand_mutex.lock();
				freopen( "log_1.log", "a", stdout );
				cout << "valid hash found " << hash << " seed is " << seed << "\n";
				fclose(stdout);
				rand_mutex.unlock();
			}
		}

	for (int i = 0; i<duration; i++)
	{
		seed = random_string();
			// BOOST_LOG_TRIVIAL(trace) << "thread " << this_thread::get_id() << " gets random seed "<< seed;
			hash = picosha2::hash256_hex_string(seed);
			cout << "thread " << this_thread::get_id() << " gets random seed "<< seed << "\n";
			// BOOST_LOG_TRIVIAL(trace) << "thread " << this_thread::get_id() << " gets hash "<< hash;
			cout << "thread " << this_thread::get_id() << " gets hash "<< hash << "\n";

			if(check_hash(hash))
			{
				// BOOST_LOG_TRIVIAL(info) << "valid hash found " << hash;
				cout << "valid hash found! " << hash << "\n";
				rand_mutex.lock();
				freopen( "log_1.log", "a", stdout );
				cout << "valid hash found " << hash << " seed is " << seed << "\n";
				fclose(stdout);
				rand_mutex.unlock();
			}
	}
}

string random_string() //создание строки HEX.
{
	string seed;
	char symbol;
	int symbol_code;
	rand_mutex.lock();
	
	
	for(int i = 0; i<hex_seed_length; i++)
	{
		// srand (time(NULL));
		symbol_code = rand()%15+'0';
		if(symbol_code<='9')
			symbol = symbol_code;
		else
			symbol = symbol_code+7;
		seed+=symbol;
	}
	rand_mutex.unlock();
	return seed;
}

bool check_hash(string hash)//проверка последних difficulty нулей хеша.
{
	for(int j = 1; j<difficulty; j++)
		if(hash.at(hex_seed_length-j)!='0')
			return false;
	return true;
}