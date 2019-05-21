using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class endgame : MonoBehaviour
{

    public GameObject player;
    // Update is called once per frame
    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.tag == "Player"){
            FindObjectOfType<GameManager>().EndGame();
        }
    }
}
