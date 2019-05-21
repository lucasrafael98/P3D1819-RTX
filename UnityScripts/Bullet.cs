using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bullet : MonoBehaviour
{
    private Transform target;
    private Vector3 initialDirection;
    public float speed = 30f;
    public GameObject impactEffect;

    public void Seek(Transform _target){
        target = _target;
        initialDirection = target.position - transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        if(target == null){
            Destroy(gameObject);
            return;
        }
        Vector3 dir = target.position - transform.position;
        float distanceThisFrame = speed * Time.deltaTime;

        if(dir.magnitude <= distanceThisFrame){
            HitTarget();
            return;
        }

        if(initialDirection != null){
            transform.Translate(initialDirection.normalized * distanceThisFrame, Space.World);
        }
    }

    void HitTarget(){
        GameObject effectIns = (GameObject)Instantiate(impactEffect,transform.position,transform.rotation);
        target.gameObject.GetComponent<PlayerHP>().updateHP(-10);
        Destroy(effectIns,2f);
        Destroy(gameObject);
    }
}
