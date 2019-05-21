using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TreasureScript : MonoBehaviour
{    
    void OnTriggerEnter(Collider other){
        if(other.tag.Equals("Player")){
            FindObjectOfType<GameManager>().treasure = true;
        }
    }
}
