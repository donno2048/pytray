import pytray, time, os
i = 0
pytray.create_tray_element()
pytray.hide_window()
# TODO: this method is not currently working
# pytray.create_menu(
#     2,
#     ["Exit", "Close"],
#     [1, 2]
# )
pytray.add_icon()
while True:
    # Not useful without a menu
    # click = pytray.what_is_clicked()
    # if click == 1:
    #   exit()
    # elif click == 2:
    #     pass
    # elif click == None:
    #     pass
    time.sleep(.1)
    pytray.set_icon(os.path.abspath(f"cat\\{i}.ico"))
    i = (i + 1) % 5