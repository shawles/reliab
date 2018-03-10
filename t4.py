from random import randint, uniform
from math import floor
from numpy.random import shuffle, choice
from numpy import arange
from lxml import etree
import os
from os import listdir
from matplotlib import pyplot as plt
import csv
import sys
import argparse


n = 2
m = 2
times = [1, 1]
max_gens = 500
max_time = 100
pop_size = 150
mut_prob = 0.1

#парсинг аргументов командной строки
def parse():
	parser = argparse.ArgumentParser()
	parser.add_argument('--f', type = str, default = 'data', 
		help = 'folder with xml data samples')
	parser.add_argument('--gen', type = int, default = 500, 
		help = 'number of generations')
	parser.add_argument('--o', type = str, default = 'res.txt', 
		help = 'file to save csv results')	
	args = parser.parse_args()
	return args


#вычисление функции выживаемости
def fitness(sch, n, m, times):
	cpu_times = [0 for i in range(m)]
	for i in range(n):
		cpu_times[sch[i] - 1] += times[i]
	#print(cpu_times)
	return sum(times) - max(cpu_times)	

#вычисление средней функции выживаемости
def mean_fitness(samples, n):
	mn = 0
	for i in range(len(samples)):
		mn += samples[i][n]
	mn *= (1. / len(samples))
	return mn

#инициализация решения случайным образом
def init_sch(n, m):
	l = [randint(1, m) for i in range(n + 1)]
	l[n] = fitness(l, n, m, times)
	return l

#операция скрещивания
#для правильного скрещивания решений р1 и р2 
#нужно вызвать cross(p1, p2), а потом cross(p2, p1)
def cross(p1, p2, n, m, times):
	child = list(p1)
	num = randint(1, n - 1)
	child[num:] = p2[num:]
	child[n] = fitness(child, n, m, times)
	return child

#операция мутации
def mutate(sch, n, m, times):
	num = randint(0, n - 1)
	old = sch[num]
	new = randint(1, m)
	while (old == new):
		new = randint(1, m)
	sch[num] = new
	sch[n] = fitness(sch, n, m, times)
	return sch


#построение списка пар родителей для генерации следующего поколения.
#Вероятность решения попасть в список определяется отношением 
#его функции выживаемости к среднему значению функции выживаемости. 
#Пары подбираются так, чтобы два одинаковых решения не шли подряд.
def cross_list(samples, n):
	cl = []
	rs = []
	mean = mean_fitness(samples, n)
	for i in range(pop_size):
		ratio = samples[i][n] / mean
		rs.append(ratio * 1. / pop_size)
	for i in range(pop_size // 2):
		c = choice(arange(0, pop_size), p = rs)
		cl.append(c)
		c2 = choice(arange(0, pop_size), p = rs)
		while c2 == c:
			c2 = choice(arange(0, pop_size), p = rs)
		cl.append(c2)
	return cl


#проверка, обязательно ли в массиве пар родителей никакое решение
#не предназначено для скрещивания с самим собой. По идее, 
#всегда должно выполняться, но для повышения надежности оставлено.
def check_diff_pairs(ar):
	l2 = len(ar) // 2
	for i in range(l2):
		if ar[2 * i] == ar[2 * i + 1]:
			return False
	return True

#генерация нового поколения по списку пар родителей: 
#берутся два родителя, по ним получаются новые потомки, которые 
#добавляются в новый массив, который и становится новым поколением.
def cross_gen(samples, cl, n, m, times):
	lcl = len(cl)
	new_gen = []
	if lcl == 1:
		new_gen.append(samples[cl[0]])
	elif lcl > 1:
		while not check_diff_pairs(cl):
			shuffle(cl)
		for i in range(pop_size // 2):
			i1 = 2 * i
			ind_p1 = cl[i1]
			i2 = 2 * i + 1
			ind_p2 = cl[i2]
			new_gen.append(cross(samples[ind_p1], samples[ind_p2], n, m, times))
			new_gen.append(cross(samples[ind_p2], samples[ind_p1], n, m, times))
			
	return new_gen


#pretty-printing лучшего из полученных решений 
#и возвращение его времени выполнения для удобства
def get_best(samples, times, m):
	l = len(samples[0])
	samples.sort(key = lambda x: x[-1])
	#лучший - последний
	for i in range(0, m):
		s = 0
		print(i + 1, ': ', sep = '', end = '')
		for j in range(len(times)):
			if samples[-1][j] == i + 1:
				s += times[j]
				print(times[j], end = ' ')
		print(' =', s)
	print(samples[-1])
	#возвращает максимальное время
	return sum(times) - samples[-1][l - 1]

#перевод условия задачи в xml
def to_xml(n, m, times):
	data = etree.Element('Data')
	cpus = etree.SubElement(data, "CPUs", num = str(m))
	for i in range(n):
		data.append(etree.Element('Job', num = str(i + 1), t = str(times[i])))
	s = etree.tostring(data)
	return s

#запись в файл xml условия задачи
def write_to_xml(n, m, times, filename):
	s = to_xml(n, m, times)
	f = open(filename, 'wb')
	f.write(s)
	f.close()	

#извлечение условия задачи из файла xml
def from_xml(st):
	data = etree.fromstring(st)
	for elem in data.iter("CPUs"):
		m = int(elem.get("num"))
	n = 0
	for elem in data.iter("Job"):
		n += 1
	times = [0 for i in range(n)]
	for elem in data.iter("Job"):
		times[int(elem.get("num")) - 1] = int(elem.get("t"))
	return n, m, times
	
#чтение из файла xml условия задачи
def read_from_xml(filename):
	f = open(filename, 'rb')
	st = f.read()
	return from_xml(st)

#генерация данных для исследования
def generate_xml(folder = 'data'):
	#будет сгенерировано coef * coef решений для каждой пары (N, M)
	#количества задач и процессоров
	coef = 5
	#максимальное время выполнения работы (минимальное - 1)
	maxtime = 10
	
	for jobs in range(5, 20, 3):
		for c1 in range(coef):
			for cpus in range(2, 7, 2):
				for c2 in range(coef):
					ts = [randint(1, maxtime) for i in range(jobs)]
					fname = folder + '//' + str(jobs) + '_' + str(cpus) + '_' + str(c1) + '_' + str(c2) + '.xml';
					write_to_xml(jobs, cpus, ts, fname)


#поиск решения конкретной задачи
def solution(n, m, times):
	global max_time 
	max_time = sum(times)

	samples = []
	for i in range(pop_size):
		samples.append(init_sch(n, m))

	gens = 0

	while (gens < max_gens):
		
		#Селекция и скрещивание
		cl = cross_list(samples, n)
		samples = cross_gen(samples, cl, n, m, times)
		
		#Мутации
		for i in range(len(samples)):
			u = uniform(0, 1)
			if u < mut_prob:
				samples[i] = mutate(samples[i], n, m, times)
		
		
		if gens == max_gens - 1:
			print()
			#print(gens, 'new_gen:', samples)
			print('best:')
			return get_best(samples, times, m)
		gens += 1

#функция для проверки гипотезы: подсчёт количества и процента
#задач, для которых удалось найти решение не хуже Tmin на num
def less_than(filename, num = 2):
	f = open(filename, 'r')
	r = csv.reader(f)
	a = 0
	g = 0
	for row in r:
		if float(row[-1]) <= num:
			g += 1
		a += 1
	print(g, a, g / a)

#функция для проверки гипотезы: подсчёт количества и процента
#задач, для которых удалось найти решение не хуже Tmin на 
#продолжительность минимальной работы
def r3_less_than(filename):
	f = open(filename, 'r')
	r = csv.reader(f)
	a = 0
	g = 0
	for row in r:
		if float(row[-1]) < float(row[3]):
			g += 1
		a += 1
	print(g, a, g / a)

#аналог функции main в С/С++
if __name__ == '__main__':
	
	#парсинг аргументов командной строки
	args = parse()
	max_gens = args.gen
	folder = args.f
	outfile = args.o
	f = open(outfile, 'w')
	wrt = csv.writer(f)
	
	#сгенерировать данные, если ещё не
	if not os.path.isdir(folder):
		os.mkdir(folder)
		generate_xml(folder)
		#exit()
		
	#перебрать все файлы xml в папке, для каждого найти решение
	#и записать результат в csv
	l = listdir(folder)
	rs = []
	for fname in l:
		if '.xml' in fname: # and '5_2_0' in fname:
			print(fname)
			n, m, times = read_from_xml(folder + '//' + fname)
			sol = solution(n, m, times)
			#время выполнения расписания
			st = sum(times)
			#sol = st - sol
			print(n, m, times, st, sol, st / m, sol - (st / m))
			r = [n, m, st, min(times), max(times), "%0.2f" % (st / m), sol, "%0.2f" % (sol - st / m)]
			rs.append(r)
			wrt.writerow(r)
			print()
	rs.sort(key = lambda x: float(x[7]))
	f.close()
	

