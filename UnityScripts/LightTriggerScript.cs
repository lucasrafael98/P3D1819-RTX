using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LightTriggerScript : MonoBehaviour
{

    void OnTriggerEnter(Collider other){
        if(other.tag.Equals("Player")){
            gameObject.GetComponentInChildren<Light>().enabled = true;
        }
    }
}
