using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class CharacterAnimator : MonoBehaviour
{
    const float locomotionAnimationSmoothTime = .1f;
    NavMeshAgent agent;
    Animator animator;

    Enemy enemy;

    // Start is called before the first frame update
    void Start()
    {
        agent = GetComponent<NavMeshAgent>();
        animator = GetComponentInChildren<Animator>();
        enemy = GetComponent<Enemy>();
        enemy.OnAttack += OnAttack;
    }

    // Update is called once per frame
    void Update()
    {
        float speedPercent = agent.velocity.magnitude / agent.speed;
        animator.SetFloat("speedPercent",speedPercent,locomotionAnimationSmoothTime,Time.deltaTime);
        animator.SetBool("inCombat",enemy.InCombat);
    }

    void OnAttack(){
        animator.SetTrigger("attack");
    }
}
