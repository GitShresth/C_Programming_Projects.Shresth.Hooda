import java.util.Scanner;
import java.util.ArrayList;
import java.util.*;
class NFAConverter {
    public static void main(String[] args) {
        Scanner myObj = new Scanner(System.in);
        //in the form "q0,q1,q2"
        System.out.println("Enter States:");
        String stateIn = myObj.nextLine(); 
        System.out.println("Enter Alphabet:");
        String alphabetIn = myObj.nextLine();
        //transitions seperated by semicolons. transitions: origin,input,target
        System.out.println("Enter Transitions:");
        String transitionIn = myObj.nextLine();
        String[] transit = transitionIn.split(";");
        System.out.println("Enter Start State:");
        String start = myObj.nextLine();
        //in the form "q1,q2"
        System.out.println("Enter Final States:");
        String finalIn = myObj.nextLine(); 
        
        boolean containsSink = false;
        String[] states = stateIn.split(",");
        ArrayList<String> statesL = new ArrayList<String>(Arrays.asList(states));
        String[] alphabet = alphabetIn.split(",");
        String[][] transitions = new String[transit.length][3];
        String[] finals = finalIn.split(",");
        ArrayList<String> finalsL = new ArrayList<String>(Arrays.asList(finals));
        ArrayList<ArrayList<String>> nTransitions = new ArrayList<ArrayList<String>>();
        
        //constructing transitions
        for(int i = 0; i < transit.length; i++) {
            transitions[i] = transit[i].split(",");
        }
        ArrayList<String> NFAStates = new ArrayList<String>();
        NFAStates.add(statesL.get(0));
        System.out.println("------------");
        System.out.println("NFA = ({" + stateIn + "},{" + alphabetIn + "},{" + transitionIn + "}," + start + ",{" + finalIn + "})");
        //get new transitions and states for DFA
        NFAConverter inst = new NFAConverter();
        
        for(int i = 0; i < NFAStates.size(); i++) {
            //check each state's output on each input
            for(int j = 0; j < alphabet.length; j++) {
                String outputState = inst.transitionState(NFAStates.get(i).split(","), alphabet[j], transitions);
                if(!outputState.equals("") && !NFAStates.contains(outputState)) {
                    NFAStates.add(outputState);
                }
                if(outputState.equals("")) {
                    outputState = "sink";
                    containsSink = true;
                }
                nTransitions.add(new ArrayList<String>(List.of(NFAStates.get(i),alphabet[j],outputState)));
            }
        }
        //get final states
        ArrayList<String> newFinals = new ArrayList<String>();
        for(int i = 0; i < NFAStates.size(); i++) {
            String[] stateDecomp = NFAStates.get(i).split(",");
            boolean isFinal = false;
            for(int j = 0; j < stateDecomp.length; j++) {
                if(finalsL.contains(stateDecomp[j])) {
                    isFinal = true;
                }
            }
            if(isFinal) {
                newFinals.add(NFAStates.get(i));
            }
        }
        System.out.println("Converting NFA to DFA...");
        System.out.print("DFA = ({");
        for(int i = 0; i < NFAStates.size(); i++) {
            if(i == NFAStates.size() - 1) {
                System.out.print(NFAStates.get(i).replace(",", ""));
            }
            else {
                System.out.print(NFAStates.get(i).replace(",", "") + ",");
            }
        }
        if(containsSink) {
            System.out.print(",sink");
        }
        System.out.print("},{" + alphabetIn + "},{");
        for(int i = 0; i < nTransitions.size(); i++) {
            if(i == nTransitions.size() - 1) {
                System.out.print(nTransitions.get(i).get(0).replace(",", "") + "," + nTransitions.get(i).get(1) + "," + nTransitions.get(i).get(2).replace(",", ""));
            }
            else {
                System.out.print(nTransitions.get(i).get(0).replace(",", "") + "," + nTransitions.get(i).get(1) + "," + nTransitions.get(i).get(2).replace(",", "") + ";");
            }
        }
        System.out.print("}," + start + ",{");
        for(int i = 0; i < newFinals.size(); i++) {
            if(i == newFinals.size() - 1) {
                System.out.print(newFinals.get(i).replace(",", ""));
            }
            else {
                System.out.print(newFinals.get(i).replace(",", "") + ",");
            }
        }
        System.out.println("})");
    }
    public String transitionState(String[] state, String input, String[][] transitions){
        String nState = "";
        for(int k = 0; k < state.length; k++) {
            for(int i = 0; i < transitions.length; i++) {
                if(transitions[i][0].equals(state[k]) && transitions[i][1].equals(input)) {
                    String[] stateDecompT = nState.split(",");
                    ArrayList<String> stateDecomp = new ArrayList<String>(Arrays.asList(stateDecompT));
                    if(!stateDecomp.contains(transitions[i][2])) {
                        nState += transitions[i][2] + ",";
                    }
                }
            }
        }
        if(!nState.equals("")) {
            nState = nState.substring(0, nState.length() - 1);
        }
        return nState;
    }
}

