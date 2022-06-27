using System.Collections;
using System.Collections.Generic;
using UnityEngine.AI;
using UnityEngine;

public class Spieler : MonoBehaviour
{
    private NavMeshAgent agent;
    private const int blickfeld = 80;//°

    // Start is called before the first frame update
    private void Start()
    {
        agent = this.GetComponentInChildren<NavMeshAgent>();
        transform.forward = new Vector3(-1, 0, 0);
    }

    // Update is called once per frame
    private void Update()
    {
        //Zeigt das Blickfeld des Spielers an
        Debug.DrawLine(transform.position, transform.position + Quaternion.Euler(0, -blickfeld / 2f, 0) * transform.forward * 3);
        Debug.DrawLine(transform.position, transform.position + Quaternion.Euler(0, blickfeld / 2f, 0) * transform.forward * 3);

        if (Input.GetMouseButtonUp(0))
        { 
            //Finde die Kollision mit dem Level
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            RaycastHit hit;
            //Prüft ob sich ein gültiges Ziel unter der Maus befindet
            Physics.Raycast(ray, out hit, Mathf.Infinity, ~(1 << LayerMask.NameToLayer("IgnoreGoTo")));

            // Zeigt Bewegungsrichtung des Spielers an
            Debug.DrawLine(transform.position, hit.point);

            //Bewegt sich zum Ziel 
            if (hit.collider.gameObject.GetComponent<Vase>() == null)
            {
                agent.destination = hit.point;
            }
            //Außer dass Ziel ist eine Vase
            else
            {
                transform.LookAt(hit.point);
            }
        }

    }

    public bool Sieht(Vase v)
    {
        Vector3 zurVase = v.transform.position - transform.position;
        float winkelZurVase = Vector3.Angle(transform.forward, zurVase);

        //Überprüfen ob Vase im Blickfeld liegt
        if (winkelZurVase < blickfeld / 2 && zurVase.magnitude < 3)
            return true;
        return false;
    }
}
