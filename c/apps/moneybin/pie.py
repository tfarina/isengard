import matplotlib.pyplot as plt

labels= ['REN', 'EDP', 'GALP', 'REPSOL', 'TELEFONICA', 'ENAGAS']

colors=['blue', 'red', 'orange', 'yellow', 'purple', 'green']

sizes= [30, 2, 8, 20, 20, 20]

plt.pie(sizes,labels=labels, colors=colors, startangle=90, autopct='%1.1f%%')

plt.axis('equal')

plt.show()
