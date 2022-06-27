using UnityEngine;

//Second person camera class

public class secondPerson : MonoBehaviour
{
    private Vector3 dragOrigin;
    private const float minZoom = 2;
    private const float maxZoom = 15;

    // Update is called once per frame
    private void Update()
    {
        //Prüft ob Kamera Atkiv ist
        if (!this.gameObject.GetComponent<Camera>().enabled)
            return;

        // Maustaste Gedrückt halten um Kamera zu "ziehen"
        if(Input.GetMouseButton(1) || Input.GetMouseButton(2))
        {
            if (Input.GetMouseButtonDown(1) || Input.GetMouseButtonDown(2))
               dragOrigin = Camera.main.ScreenToWorldPoint(Input.mousePosition);

            Vector3 diff = dragOrigin - Camera.main.ScreenToWorldPoint(Input.mousePosition);

            Camera.main.transform.position += diff;
        }


        //Vector3 tmp = Camera.main.transform.forward;
        float zoomChange = 0;

        //Tasten um Zoom zu verändern
        if (Input.GetKeyDown(KeyCode.KeypadPlus))
            zoomChange = -Camera.main.orthographicSize / 10;
        else if(Input.GetKeyDown(KeyCode.KeypadMinus))
            zoomChange = +Camera.main.orthographicSize / 10;

        //Zoom verändern per Mausrad
        else if(Input.GetAxis("Mouse ScrollWheel") != 0f)
        {
            if(Input.GetAxis("Mouse ScrollWheel") > 0)
                zoomChange = -Camera.main.orthographicSize / 10;
            else
                zoomChange = +Camera.main.orthographicSize / 10;
        }

        zoomChange += Camera.main.orthographicSize;
        //Zoom in definiertem Bereich halten
        zoomChange = Mathf.Clamp(zoomChange, minZoom, maxZoom);

        //Änderungen anwenden
        Camera.main.orthographicSize = zoomChange;
    }
}
