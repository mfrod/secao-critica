package ex3;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import javax.swing.JOptionPane;

public class TrafficController {
    final Lock trava = new ReentrantLock();
    final Condition Vazia = trava.newCondition();

    int ponteEstado = 0; // 0 vazia, 1 cheia    **variavel de controle**
    public  void enterLeft(){
        trava.lock();
        try {
                if ((ponteEstado != 0)) Vazia.await();
            
        } catch (InterruptedException ie) {
            System.err.println(ie.toString());
            //TODO: handle exception
        }
        ponteEstado = 1;
        //JOptionPane.showMessageDialog(null, "Cheguei na ponte");
        trava.unlock();
    }
    public void enterRight() {
        trava.lock();
        try {
                if ((ponteEstado != 0)) Vazia.await();
            
        } catch (InterruptedException ie) {
            System.err.println(ie.toString());
            //TODO: handle exception
        }
        ponteEstado = 1;
        trava.unlock();
    }
    public void leaveLeft() {
        trava.lock();
        ponteEstado = 0;
        Vazia.signal();
        trava.unlock();
    }
    public void leaveRight() {
        trava.lock();
        ponteEstado = 0;
        Vazia.signal();
        trava.unlock();
    }


    // public  void enterLeft(){
    // }
    // public void enterRight() {
    // }
    // public void leaveLeft() {
    // }
    // public void leaveRight() {
    // }




}