
import java.util.ArrayList;
import java.util.concurrent.TimeUnit;

/**
 *
 * @author dtgmu
 */
public class Algorithm {

    public static void main(String[] args) throws InterruptedException {

        // Clinic object itself
        Clinic clinic = new Clinic();

        // Parallel queues
        Queue Queue_App = new Queue(); 
        Queue Queue_NonApp = new Queue();

        // Lists used throughout the work day
        ArrayList<PatientPhysicianPair> PatientWithPhysician=new ArrayList<>();
        ArrayList<Physician> AvailablePhysicians = new ArrayList<>();

        // Used at the end of day
        ArrayList<Patient> FinishedPatients = new ArrayList<>();
        ArrayList<Patient> CancelledPatients = new ArrayList<>();

        while (clinic.open && clinic.acceptingPatients) {

            // Grab patients from incoming patients list
            if (!clinic.incomingPatients.isEmpty()) {
                for (Patient p : clinic.incomingPatients) {
                    if (p.SCHEDULED.isAppointment()) {
                        Queue_App.add(p);
                    } else {
                        Queue_NonApp.add(p);
                        // Ideally we would determine patient's estimated time 
                        // arrival based on their location and when they would 
                        // leave
                    }
                }
            }

            // Check if any patients have switched their status to cancelled
            if (!clinic.calcellations.isEmpty()) {
                clinic.calcellations.forEach((p) -> {
                    if (p.SCHEDULED.isAppointment()) {
                        Queue_App.dequeue(p);
                    } else {
                        Queue_NonApp.dequeue(p);
                    }
                });
            }

            // Check if any patients are over 15 minutes late
            // TODO
            // Check if patients with physicians are now done
            if (!PatientWithPhysician.isEmpty()) {

                ArrayList<PatientPhysicianPair> toRemove = new ArrayList<>();

                for (PatientPhysicianPair pair : PatientWithPhysician) {
                    if (pair.isDone()) {
                        // Add patient to finished
                        FinishedPatients.add(pair.patient);

                        // Invoke out time for patient
                        pair.patient.out();

                        // Add Physician to toRemove --> not wise to remove 
                        // inside loop
                        toRemove.add(pair);

                        // Add physician to available
                        AvailablePhysicians.add(pair.physician);

                    }
                }

                // Detaching pair between patient and physician
                toRemove.forEach((p) -> {
                    PatientWithPhysician.remove(p);
                });
            }

            // ================================================================
            // Check if we can remove a patient from either queue
            // Check if physicians are available first
            if (!AvailablePhysicians.isEmpty()) {

                // Returns corresponding patient of which would go first
                Patient next = Queue_App.peek().compareTo(Queue_NonApp.peek());

                // Need to know which queue patient came from
                if (next.SCHEDULED.isAppointment()) {
                    Queue_App.dequeue();

                    // Add physician to new list, remove from available list
                    PatientWithPhysician.add(
                            new PatientPhysicianPair(
                                    next, next.physician,
                                    java.time.LocalTime.now()));
                    AvailablePhysicians.remove(next.physician);

                } else {

                    /* Check if Queue_NonApp.peek().reasonForVisit.length will
                     * conflict with any patient's appointment time with that
                     * physician in Queue_App
                     */
                    // iterate through all available physicians
                    Patient other;

                    for (Physician AvailablePhysician : AvailablePhysicians) {

                        // Iterate through all Queue_App patients
                        for (int i = 0; i < Queue_App.size(); i++) {
                            other = Queue_App.peek(i);

                            // Check if this potentially "next" patient's 
                            // physician == other's physician AND
                            // Check if the times would conflict
                            if (!next.physician.equals(other.physician) 
                                    && !next.compareTo(other).equals(other)) {

                                // Dequeue visiting patient from visit queue
                                Queue_NonApp.dequeue();

                                // Add physician to new list, remove from 
                                // available list
                                PatientWithPhysician.add(
                                        new PatientPhysicianPair(
                                                next, 
                                                next.physician, 
                                                java.time.LocalTime.now()));
                                AvailablePhysicians.remove(next.physician);
                            }
                        }
                    }
                    /**
                     * If we step out of loop without de-queue from QueueVisit
                     * then we know there was a conflict so patients wait
                     */

                    
                } // End inner else - statement

            } // End outer if - statement

            // Calculate current Estimated Wait Times for each queue
            for (int i = 0; i < Queue_App.size(); i++) {
                Queue_App.peek(i).setWait();
            }
            for (int i = 0; i < Queue_NonApp.size(); i++) {
                Queue_NonApp.peek(i).setWait();
            }

            // Sleep for 5 seconds
            TimeUnit.SECONDS.sleep(5);

        } // End while loop
    }
}
