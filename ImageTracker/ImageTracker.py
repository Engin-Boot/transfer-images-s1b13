import pandas as pd
import sys
import os
import csv


def write_csv():                 #writes empty csv consisting only column names  
    filename="DiagnosisTracker.csv"
    fields=['ImageFileName','Status']
    with open(filename, 'w') as csvfile: 
        # creating a csv writer object  
        csvwriter = csv.writer(csvfile)  
        # writing the fields  
        csvwriter.writerow(fields)  


def addNewFileNameToCsv(file):     #add new row(new Image File) to CSV
    diagnosis_data=pd.read_csv('DiagnosisTracker.csv')
    new_row = {'ImageFileName': file, 'Status':'Diagnosis Pending'}
    diagnosis_data = diagnosis_data.append(new_row, ignore_index=True)
    diagnosis_data.to_csv('DiagnosisTracker.csv',index=False)


def showCSVData():                #print CSV file data
    print("CSV file Diagnosis data:")
    diagnosis_data=pd.read_csv('DiagnosisTracker.csv')
    print(diagnosis_data)


def check_new_file_and_update_in_csv(images_path):    #check new image file and add to csv file
    listOfImages=os.listdir(images_path)
    DiagnosisData=pd.read_csv('DiagnosisTracker.csv')
    current_list=DiagnosisData['ImageFileName'].values.tolist()
    for i in listOfImages:
        if(i not in current_list):
            addNewFileNameToCsv(i)


def update_ImageStatus(imageName,updated_status):    #update respective image's Diagnosis status
    DiagnosisData=pd.read_csv('DiagnosisTracker.csv')
    if updated_status=="yes":
        updated_status="Diagnosis Complete"
    else:
        updated_status="Pending"
    DiagnosisData.loc[DiagnosisData['ImageFileName'] == imageName, 'Status'] = updated_status
    DiagnosisData.to_csv('DiagnosisTracker.csv')
    print("Status updated successfully")
    

def startImageTracking():      #start image tracking 
    showCSVData()
    while(1):
        print("Enter Y/y for continuing image tracking or N/n to stop ")
        x=input()
        if(x=='Y' or x=='y'):
            print("Image Tracking started")
            print("Enter image Name and status(Yes for Diagnosis Complete/No for Pending)")
            imageName=input()
            updated_status=input()
            update_ImageStatus(imageName,updated_status)
            showCSVData()
        else:
            print("Image Tracking stopped")
            exit()



if __name__ == "__main__":
    images_path=sys.argv[1]     #Path of all images folder
    write_csv()                 #writes empty csv consisting only column names     
    check_new_file_and_update_in_csv(images_path)  #check new image file and update in CSV 
    print("CSV File checked and updated")
    startImageTracking()        #start tracking images    