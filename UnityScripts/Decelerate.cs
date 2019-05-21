using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Decelerate : MonoBehaviour
{
    public float decelFactor = 0.5f;

    // Update is called once per frame
    void Update()
    {
        float x = gameObject.GetComponent<Rigidbody>().velocity.x;
        float y = gameObject.GetComponent<Rigidbody>().velocity.y;
        float z = gameObject.GetComponent<Rigidbody>().velocity.z;
        if(x != 0 || y != 0 || z != 0) gameObject.GetComponent<Rigidbody>().velocity = new Vector3(decelFactor * x, decelFactor * y, decelFactor * z);
    }
}
