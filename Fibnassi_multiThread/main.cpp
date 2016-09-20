/*
 * main.cpp
 *
 *  Created on: Sep 13, 2016
 *      Author: erwin
 */

#include <iostream>
#include <thread>
#include "queue.h"

using namespace std;


int fib(int n)
{
    if((n==1)||(n==0))
    {
        return(n);
    }
    else
    {
        return(fib(n-1)+fib(n-2));
    }
}

void run(Queue<int> * queue)
{
	while(true){
		int ass = queue->pop();
		cout << "Assigment " << ass << " Recieved" << endl;

		if (ass == 0){
			cout << "Thread ended" << endl;
			return;
		}
		int ans = fib(ass);
		cout << "Fib(" << ass << ")=" << ans << endl;
	}

}

int main()
{
	int answ = fib(10);
	cout << "answ=" << answ << endl;
	Queue<int> queue;


	for(int i = 5; i < 60; i+=1)
			queue.push(i);

	queue.push(0);
	queue.push(0);
	queue.push(0);
	queue.push(0);

	std::thread t1(&run, &queue);
	std::thread t2(&run, &queue);
	std::thread t3(&run, &queue);
	std::thread t4(&run, &queue);


	t1.join();
	t2.join();
	t3.join();
	t4.join();

	cout << "Main thread end" << endl;
}
