import re
from root import *
from PIL import Image, ImageTk
import serial.tools.list_ports
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator
from matplotlib.animation import FuncAnimation
from matplotlib.backends.backend_tkagg import (
    FigureCanvasTkAgg,
    NavigationToolbar2Tk
)
import pandas as pd

# Serial Com
serial_arduino = serial.Serial(baudrate=57600, timeout=0)

# Port Frame Base
main_port_frame = Frame(
    root,
    relief=RIDGE,
    bd=2,
    background="white",
    highlightbackground="black",
    highlightthickness=2
)
main_port_frame.columnconfigure(0, weight=1)
main_port_frame.columnconfigure(1, weight=8)
main_port_frame.rowconfigure(tuple(range(5)), weight=1)
main_port_frame.grid(row=0,  column=0, padx=10,  pady=10, sticky=N+S+E+W)


# Left Side Port Frame
port_frame = Frame(
    main_port_frame,
    relief=RIDGE,
    bd=2,
    background="white",
    highlightbackground="black",
    highlightthickness=2,
    width=100
)
port_frame.grid(row=0,  column=0, rowspan=5, padx=5,  pady=5, sticky=N+S+E+W)
port_frame.columnconfigure(0, weight=1)
port_frame.columnconfigure(1, weight=1)
port_frame.rowconfigure(tuple(range(99)), weight=1)

# Title App
title_app = Label(port_frame, text="PID Controller", font=(
    'Times 16 bold'), background="yellow").grid(row=0, column=0, columnspan=2, padx=5,   pady=5, sticky=N+W+E)

# Option Ports
ports = [str(port.device) for port in serial.tools.list_ports.comports()]
ports = tuple(ports)
port_value = StringVar(port_frame)
port_value.set("Select Ports")
port_selection = ttk.Combobox(port_frame, textvariable=port_value)
port_selection['value'] = ports
port_selection.grid(row=1, column=0, columnspan=2,
                    padx=5, sticky=N+E+W)

# Image
image = Image.open("logo.png")
basewidth = 200
wpercent = (basewidth/float(image.size[0]))
hsize = int((float(image.size[1])*float(wpercent)))
image = image.resize((basewidth, hsize), Image.Resampling.LANCZOS)
image = ImageTk.PhotoImage(image)
brin_img = Label(port_frame, image=image).grid(
    row=99, column=0, columnspan=2, sticky=S+W+E)


# Refresh ports Button
def update_ports():
    ports = [str(port.device) for port in serial.tools.list_ports.comports()]
    ports = tuple(ports)
    port_selection['value'] = ports


refresh_port_button = Button(
    port_frame, text="Refresh Ports", activebackground='#00ff00', highlightbackground="red", command=update_ports).grid(row=2, columnspan=2, column=0, padx=5, sticky=W+E)


# Showing Ports Text
showing_port_text = Text(
    port_frame,
    height=1,
    width=25,
    bg="white"
)
showing_port_text.grid(row=3, column=0, columnspan=2, padx=5, sticky=E+W)
showing_port_text.tag_configure("tag_name", justify='center')
showing_port_text.insert("1.0", 'Port is not selected')
showing_port_text.tag_add("tag_name", "1.0", "end")
showing_port_text.tag_add("center", "1.0", "end")


def update_text_port(event):
    # if you want to remove the old data
    showing_port_text.delete(1.0, END)
    serial_arduino.port = port_value.get()
    try:
        serial_arduino.open()
        showing_port_text.tag_configure("tag_name", justify='center')
        showing_port_text.insert(
            "1.0", 'Port : ' + port_value.get() + " (Connected)")
        showing_port_text.tag_add("tag_name", "1.0", "end")
        showing_port_text.tag_add("center", "1.0", "end")
        return True
    except (FileNotFoundError, OSError):
        serial_arduino.close()
        showing_port_text.tag_configure("tag_name", justify='center')
        showing_port_text.insert(
            "1.0", 'Port : ' + port_value.get() + " (Not Connected)")
        showing_port_text.tag_add("tag_name", "1.0", "end")
        showing_port_text.tag_add("center", "1.0", "end")
        return False


port_selection.bind('<<ComboboxSelected>>', update_text_port)

# Calibration Status


show_cal_status = True


def start_measure():
    global show_cal_status
    show_cal_status = False
    RL = []
    RS = []
    RD = []
    RR = []
    RP = []
    RY = []
    serial_arduino.write("MEASURE".encode('utf-8'))
    start_measure_button.config(bg='red')
    table_calibration.delete(*table_calibration.get_children())


start_measure_button = Button(
    port_frame, text="Start Measure",
    activebackground='#00ff00',
    command=start_measure
)
start_measure_button.grid(row=17,  columnspan=2, padx=5, column=0, sticky=W+E)


# Upper Right Side For Status
upper_frame = Frame(
    main_port_frame,
    relief=RIDGE,
    bd=2,
    background="white",
    highlightbackground="black",
    highlightthickness=2
)
upper_frame.grid(row=0,  column=1, rowspan=2, padx=5,  pady=5, sticky=N+S+E+W)
upper_frame.columnconfigure(0, weight=1)
upper_frame.rowconfigure(0, weight=1)

# create the graph
x = []
RL = []
RS = []
RD = []
RR = []
RP = []
RY = []

fig, ax = plt.subplots()
plt.xlim(0, 1000)
plt.ylim(0, 150)
plt.subplots_adjust(hspace=0)


def animate(RL, RS, RD, RR, RP, RY):
    ax.clear()
    ax.plot(x, RD, label='Drag', linewidth=1)
    ax.plot(x, RP, label='Pitch', linewidth=1)
    ax.plot(x, RR, label='Roll', linewidth=1)
    ax.plot(x, RS, label='Side', linewidth=1)
    ax.set_title("Force Balance")
    ax.legend(['Drag', 'Pitch', 'Roll', 'Side'])
    ax.yaxis.set_major_locator(MaxNLocator(prune='lower'))
    plotcanvas.draw()


plotcanvas = FigureCanvasTkAgg(fig, upper_frame)
plotcanvas.get_tk_widget().grid(column=0, row=0, sticky=N+S+E+W)


# Bottom Right Side For Status
bottom_frame = Frame(
    main_port_frame,
    relief=RIDGE,
    bd=2,
    background="white",
    highlightbackground="black",
    highlightthickness=2
)
bottom_frame.grid(row=2,  column=1, rowspan=3, padx=5,  pady=5, sticky=N+S+E+W)
bottom_frame.columnconfigure(0, weight=1)
bottom_frame.rowconfigure(0, weight=1)

# Table
# RS, RP, RL, RR, RY, RD
columns = ('Time', 'Distance')
# columns = ('Yaw', 'Pitch', 'Roll', 'Lift', 'Drag', 'Side')
table_calibration = ttk.Treeview(
    bottom_frame, columns=columns, show='headings')
table_calibration.heading('Time', text='Pitch')
# table_calibration.heading('Roll', text='Roll')
table_calibration.heading('Distance', text='Distance')
# table_calibration.heading('Side', text='Side')
col_width = table_calibration.winfo_width()
table_calibration.column("# 1", anchor=CENTER, width=col_width)
table_calibration.column("# 2", anchor=CENTER, width=col_width)
table_calibration.grid(row=0,  column=0, sticky=N+S+E+W)


scrollbar = Scrollbar(
    bottom_frame,
    orient=VERTICAL,
    command=table_calibration.yview,
    activebackground='#00ff00'
)
table_calibration.configure(yscroll=scrollbar.set)
scrollbar.grid(row=0, column=0, sticky=N+S+E)

# Save To Excel


def save_as_file():
    lst = []
    col = ['Yaw', 'Pitch', 'Lift', 'Drag']
    if (len(table_calibration.get_children()) < 1):
        messagebox.showinfo("Empty Data")
    else:
        filename = filedialog.asksaveasfilename(
            initialdir=os.getcwd(),
            title="Save To Excel",
            filetypes=(("xlsx File", "*.xlsx"), ("All Files", "*.*"))
        )

        with open('temp.csv', mode='w', newline='') as f:
            csvwriter = csv.writer(f, delimiter=',')
            for row_id in table_calibration.get_children():
                row = table_calibration.item(row_id, 'values')
                lst.append(row)
            lst = list(map(list, lst))
            lst.insert(0, col)
            for row in lst:
                csvwriter.writerow(row)
        writer = pd.ExcelWriter(filename + '.xlsx')
        df = pd.read_csv('temp.csv', delimiter=',')
        df.to_excel(writer, 'sheetname')
        writer.save()


save_to_excel_button = Button(
    bottom_frame, text="Save To Excel", activebackground='#00ff00', command=save_as_file).grid(row=1, padx=5, column=0, sticky=W+E)

i = 0
# Update loop


def is_empty_or_blank(msg):
    """ This function checks if given string is empty
     or contain only shite spaces"""
    return re.search("^\s*$", msg)


stat = False
while True:
    main_port_frame.update()
    if serial_arduino.isOpen():
        input_data = serial_arduino.readline().strip().decode("utf-8")
        if len(input_data) != 0:
            print(input_data)
