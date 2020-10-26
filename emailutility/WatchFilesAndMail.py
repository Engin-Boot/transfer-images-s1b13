import time
from watchdog.observers import Observer
from watchdog.events import PatternMatchingEventHandler
import smtplib, ssl

port = 465  # For SSL
smtp_server = "smtp.gmail.com"
#sender_email = "youraddress@gmail.com"  # Enter your address
#receiver_email = "receiveraddress@gmail.com"  # Enter receiver address
#password = ""
sender_email = "karrsp01@gmail.com"  # Enter your address
receiver_email = "salonimadhan97@gmail.com"  # Enter receiver address
password = ""



context = ssl.create_default_context()

if __name__ == "__main__":
    patterns = ["*.img"]
    ignore_patterns = ""
    ignore_directories = False
    case_sensitive = True
    print("inside main")

    my_event_handler = PatternMatchingEventHandler(patterns, ignore_patterns, ignore_directories, case_sensitive)

def on_created(event):
    print(f"Hey {event.src_path} image file created!")
    message = """\
        Subject: Image File created

        New Image file Received Please check"""
               
    with smtplib.SMTP_SSL(smtp_server, port, context=context) as server:
        server.login(sender_email, password)
        server.sendmail(sender_email, receiver_email, message)

def on_deleted(event):
    print(f"Hey Someone deleted {event.src_path}!")
    message = """\
        Subject: Image file deleted

        Image file has been deleted"""
    with smtplib.SMTP_SSL(smtp_server, port, context=context) as server:
        server.login(sender_email, password)
        server.sendmail(sender_email, receiver_email, message)

def on_modified(event):
    print(f"hey buddy, {event.src_path} has been modified")
    message = """\
        Subject: Image File Modified

        Image file has been Modified"""
    with smtplib.SMTP_SSL(smtp_server, port, context=context) as server:
        server.login(sender_email, password)
        server.sendmail(sender_email, receiver_email, message)


my_event_handler.on_created = on_created
my_event_handler.on_deleted = on_deleted
my_event_handler.on_modified = on_modified

path = "."
go_recursively = True
my_observer = Observer()
my_observer.schedule(my_event_handler, path, recursive=go_recursively)

my_observer.start()
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    my_observer.stop()
    my_observer.join()
