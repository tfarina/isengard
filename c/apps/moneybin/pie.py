import matplotlib.pyplot as plt

labels= ['REN', 'REPSOL', 'TELEFONICA', 'ENAGAS', 'GALP', 'NOS']

colors=['blue', 'red', 'orange', 'yellow', 'purple', 'green']

sizes= [42.65, 16.20, 16.19, 16.19, 6.49, 2.28]

plt.pie(sizes,labels=labels, colors=colors, startangle=90, autopct='%1.1f%%')

plt.axis('equal')

plt.show()
