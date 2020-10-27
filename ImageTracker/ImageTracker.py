import pandas as pd
import sys
import os
import csv

DiagnosisTrackerfilename="DiagnosisTracker.csv"

def write_csv(DiagnosisTrackerfilename):                 #writes empty csv consisting only column names  
    fields=['ImageFileName','Status']
    with open(DiagnosisTrackerfilename, 'w') as csvfile: 
        # creating a csv writer object  
        csvwriter = csv.writer(csvfile)  
        # writing the fields  
        csvwriter.writerow(fields)  


def addNewFileNameToCsv(DiagnosisTrackerfilename,new_file):    #add new row(new Image File) to CSV
    diagnosis_data=pd.read_csv(DiagnosisTrackerfilename)
    new_row = {'ImageFileName': new_file, 'Status':'Diagnosis Pending'}
    diagnosis_data = diagnosis_data.append(new_row, ignore_index=True)
    diagnosis_data.to_csv(DiagnosisTrackerfilename,index=False)


def showCSVData(DiagnosisTrackerfilename):                 #print CSV file data
    print("CSV file Diagnosis data:")
    diagnosis_data=pd.read_csv(DiagnosisTrackerfilename)
    print(diagnosis_data)
    return "CSV data printed successfully"


def check_new_file_and_update_in_csv(images_path):         #check new image file and add to csv file
    listOfImages=os.listdir(images_path)
    DiagnosisData=pd.read_csv(DiagnosisTrackerfilename)
    current_list=DiagnosisData['ImageFileName'].values.tolist()
    for i in listOfImages:
        if(i not in current_list):
            addNewFileNameToCsv(DiagnosisTrackerfilename,i)


def update_ImageStatus(DiagnosisTrackerfilename,imageName,updated_status):    #update respective image's Diagnosis status
    DiagnosisData=pd.read_csv(DiagnosisTrackerfilename)
    if updated_status=="Complete":
        updated_status="Diagnosis Complete"
    else:
        updated_status="Diagnosis Pending"
    DiagnosisData.loc[DiagnosisData['ImageFileName'] == imageName, 'Status'] = updated_status
    DiagnosisData.to_csv(DiagnosisTrackerfilename)
    print("Status updated successfully")
    return True
    

def startImageTracking():      #start image tracking 
    while(1):
        print("Enter Y/y for continuing image tracking or N/n to stop ")
        x=input()
        if(x=='Y' or x=='y'):
            print("Image Tracking started")
            print("Enter image Name and status(Enter Complete for Diagnosis Complete/Pending for Diagnosis Pending)")
            imageName=input()
            updated_status=input()
            if(update_ImageStatus(DiagnosisTrackerfilename,imageName,updated_status)):
                print("Updated CSV data")
                y=showCSVData(DiagnosisTrackerfilename)
        else:
            print("Image Tracking stopped")
            exit()



if __name__ == "__main__":
    images_path=sys.argv[1]     #Path of all images folder
    write_csv(DiagnosisTrackerfilename)                 #writes empty csv consisting only column names     
    check_new_file_and_update_in_csv(images_path)  #check new image file and update in CSV 
    print("CSV File checked and updated")
    y=showCSVData(DiagnosisTrackerfilename)               # show existing CSV data
    startImageTracking()        #start tracking images    
