from random import randint, uniform
from math import floor
from numpy.random import shuffle, choice
from numpy import arange
from lxml import etree


n = 2
m = 2
times = [1, 1]
max_gens = 100
max_time = 100
pop_size = 6
mut_prob = 0.6

def fitness(sch):
	cpu_times = [0 for i in range(m)]
	for i in range(n):
		cpu_times[sch[i] - 1] += times[i]
	return max_time - max(cpu_times)	

def mean_fitness(samples):
	mn = 0
	for i in range(len(samples)):
		mn += samples[i][n]
	mn *= (1. / len(samples))
	return mn

def init_sch():
	l = [randint(1, m) for i in range(n + 1)]
	l[n] = fitness(l)
	return l

#for proper crossing use cross(p1, p2), then cross(p2, p1)
def cross(p1, p2):
	child = list(p1)
	num = randint(1, n - 1)
	child[num:] = p2[num:]
	child[n] = fitness(child)
	return child

def mutate(sch):
	num = randint(0, n - 1)
	old = sch[num]
	new = randint(1, m)
	while (old == new):
		new = randint(1, m)
	sch[num] = new
	sch[n] = fitness(sch)
	return sch

def check_conv(samples):
	last = samples[0][n]
	i = 1
	while (i < pop_size and samples[i][n] == last):
		i += 1
	if i != pop_size:
		return False
	return True

def cross_list(samples):
	cl = []
	rs = []
	mean = mean_fitness(samples)
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

def all_same(ar):
	s = ar[0]
	i = 1
	la = len(ar)
	while i < la and ar[i] == s:
		i += 1
	if i == la:
		return True
	else:
		return False

def check_diff_pairs(ar):
	l2 = len(ar) // 2
	for i in range(l2):
		if ar[2 * i] == ar[2 * i + 1]:
			return False
	return True

def cross_gen(samples, cl):
	lcl = len(cl)
	new_gen = []
	#print('cl:', cl)
	if lcl == 1:
		new_gen.append(samples[cl[0]])
	elif lcl > 1:
		while not check_diff_pairs(cl):
			shuffle(cl)
		#print('cl2:', cl)
		for i in range(pop_size // 2):
			i1 = 2 * i
			ind_p1 = cl[i1]
			i2 = 2 * i + 1
			ind_p2 = cl[i2]
			#print('lcl:', len(cl), lcl)
			#print('i:', i1, i2, cl[i1], cl[i2])
			new_gen.append(cross(samples[ind_p1], samples[ind_p2]))
			new_gen.append(cross(samples[ind_p2], samples[ind_p1]))
			
	return new_gen

def to_xml(n, m, times):
	data = etree.Element('Data')
	cpus = etree.SubElement(data, "CPUs", num = str(m))
	for i in range(n):
		data.append(etree.Element('Job', num = str(i + 1), t = str(times[i])))
	s = etree.tostring(data)
	return s

def write_to_xml(n, m, times, filename):
	s = to_xml(n, m, times)
	f = open(filename, 'wb')
	f.write(s)
	f.close()	


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
	#print(etree.tostring(data, pretty_print = True))

if __name__ == '__main__':
	print('Enter jobs number and CPUs number')
	n, m = map(int, input().split())
	print('Enter time for every job')
	times = list(map(int, input().split()))
	
	nm = 'tmp.xml'
	f = open(nm, 'rb')
	s = f.read()
	print(from_xml(s))
	#print(from_xml(input()))
	exit()
	
	
	max_time = sum(times) + 1

	samples = []
	for i in range(pop_size):
		samples.append(init_sch())

	gens = 0

	while (gens < max_gens):
		#Размножение
		
		cl = cross_list(samples)
		samples = cross_gen(samples, cl)
		
		#Мутации
		for i in range(len(samples)):
			u = uniform(0, 1)
			if u < mut_prob:
				samples[i] = mutate(samples[i])
		
		#Отбор
		
		print()
		print(gens, 'new_gen:', samples)
		
		#Проверка сходимости
		if check_conv(samples):
			break
		gens += 1

print('\n\n', samples)
