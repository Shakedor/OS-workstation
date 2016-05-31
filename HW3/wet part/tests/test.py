import re
import os
import math

def is_prime(n):
	return all(n % i for i in range(3, int(math.sqrt(n)) + 1, 1))

def main():

	dir = os.getcwd()
	log_files = [f for f in os.listdir(dir) if re.match("thread-*",f)] + ["primes.log"]

	numbers = []
	for log_file in log_files:
		raw_data = open(os.path.join(dir,log_file),"r").read()
		if len(raw_data) == 0:
			continue
		data = raw_data.split("\n")
		numbers = numbers + [int(num) for num in data if ((not re.match("prime*",num)) and (not num == ''))]
		
	numbers.sort()
	N = int(raw_input("Please enter the N you gave your program: "))
	for i in range(2,N+1,1):
		if numbers[i-2] != i:
			print "You are missing some numbers, or got to many of some others " "."
			quit()
				
	if len(numbers) != (N-1):
		print "You got numbers that are out of range"
		quit()
		
	print "You can go to the beach now. Just kidding, go do some Algo, you finished with threads."

if __name__ == "__main__":
	main()
