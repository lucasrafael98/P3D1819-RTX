using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AmmoScript : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        gameObject.GetComponent<Rigidbody>().AddTorque(0,100,0);
    }

    void OnTriggerEnter(Collider other){
        if(other.tag.Equals("Player")){
            GameObject.FindGameObjectWithTag("Weapon").GetComponent<GunScript>().bulletsIHave += 100;
            Destroy(gameObject);
        }
    }
}
