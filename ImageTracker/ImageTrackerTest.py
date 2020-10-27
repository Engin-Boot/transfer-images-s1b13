import unittest
import ImageTracker


class TestImageTracker(unittest.TestCase):   
    def test_when_client_calls_with_valid_csv_then_print_CSV_data(self):
        testsamplefilename = 'temp-samples-test.csv'
        with open(testsamplefilename, 'w') as samplefile:            
            samplefile.write(
"""ImageFileName,Status
0.img,Diagnosis Pending
1.img,Diagnosis Complete
2.img,Diagnosis Pending""")

        x=ImageTracker.showCSVData(testsamplefilename)
        self.assertEqual(x,"CSV data printed successfully")   


    def test_when_client_updates_Diagnosis_status_Pending_then_DiagnosisPending_is_set(self):
        testsamplefilename = 'temp-samples-test.csv'
        with open(testsamplefilename, 'w') as samplefile:            
            samplefile.write(
"""ImageFileName,Status
0.img,Diagnosis Pending
1.img,Diagnosis Pending
2.img,Diagnosis Pending""")
        imageName="0.img"
        updated_status="Pending"   # Pending for Diagnosis Pending
        self.assertTrue(ImageTracker.update_ImageStatus(testsamplefilename,imageName,updated_status)==True)


    def test_when_client_updates_Diagnosis_status_Pending_then_DiagnosisCompleted_is_set(self):
        testsamplefilename = 'temp-samples-test.csv'
        with open(testsamplefilename, 'w') as samplefile:            
            samplefile.write(
"""ImageFileName,Status
0.img,Diagnosis Pending
1.img,Diagnosis Pending
2.img,Diagnosis Pending""")
        imageName="1.img"
        updated_status="Completed"   # Complete for Diagnosis Complete
        self.assertTrue(ImageTracker.update_ImageStatus(testsamplefilename,imageName,updated_status)==True)


    def test_when_new_file_is_updated_then_it_gets_added_to_csv_with_Pending_Status(self):
        testsamplefilename = 'temp-samples-test.csv'
        with open(testsamplefilename, 'w') as samplefile:            
            samplefile.write(
"""ImageFileName,Status
0.img,Diagnosis Pending
1.img,Diagnosis Complete
2.img,Diagnosis Pending""")
        new_file="3.img"
        ImageTracker.addNewFileNameToCsv(testsamplefilename,new_file)
        print("File Added successfully")
        x=ImageTracker.showCSVData(testsamplefilename)
        self.assertEqual(x,"CSV data printed successfully")   

        
if __name__ == '__main__':    
    unittest.main()
