import numpy as np

# annual 3D printer unit sales from 2007-2011
printer_sales = [66, 355, 1816, 23265]

# calculate 3D printer growth rate
rates = []
for i in range(len(printer_sales) - 1):
    r = (printer_sales[i+1] - printer_sales[i]) * 1.0 / printer_sales[i]
    rates.append(r)

printer_growth_rate = np.mean(rates)
std = np.std(rates)
printer_growth_rate_CI = [printer_growth_rate - 2 * std,
                          printer_growth_rate + 2 * std]

# total units on kickstarter that CADScan sold. Mode of price was 650 lbs
CADScan_kickstarter_units = 50 + 81 + 2 + 6 + 3 + 2 + 3

# total unit sales of photon on indiegogo up until 4/21/2013 (people still buying)
photon_indiegogo_units = 100 + 200*3 + 196

# 8.1 million in revenue. http://www.referenceusa.com/UsBusiness/Detail/All/04297cfac8b24a6da7bfc46567bba6d0/0
nextengine_revenue_2012 = 8100000
nextengine_unit_cost = 3000
nextengine_units_2012 = nextengine_revenue_2012 * 1.0 / nextengine_unit_cost
