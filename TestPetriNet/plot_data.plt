# Set the title of the graph
set title "Total Number of Vehicles for In and Out Routes"

# Set labels for the x and y axes
set xlabel "Route"
set ylabel "Number of Vehicles"

# Specify the plot type (lines) and the data files to use
plot "in_route_data.txt" using 1:3 with lines title "Total Vehicles In", \
     "out_route_data.txt" using 1:3 with lines title "Total Vehicles Out"
