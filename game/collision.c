#include"main.h"


// Returns true if the point p is inside the rectangle r
//int pointInRect(Vec2 p, Rect r) {
//    return p.x >= r.position.x && p.x <= r.position.x + r.size.x &&
//        p.y >= r.position.y && p.y <= r.position.y + r.size.y;
//}
//
//// Returns true if the two rectangles r1 and r2 overlap
//int rectsOverlap(Rect r1, Rect r2) {
//     int ret =  pointInRect(r1.position, r2) || pointInRect(r1.position, r2);
//     return ret;
//}

// Returns the point at which the ray intersects the rectangle, or
// (0, 0) if there is no intersection
Vec2 intersectRect(Ray r, Rect rect) {
    Vec2 p1 = r.origin;
    Vec2 p2 = r.origin;
    p2.x += r.direction.x * r.length;
    p2.y += r.direction.y * r.length;

    Vec2 i1, i2;
    i1.x = fmaxf(p1.x, rect.position.x);
    i1.y = fmaxf(p1.y, rect.position.y);
    i2.x = fminf(p2.x, rect.position.x + rect.size.x);
    i2.y = fminf(p2.y, rect.position.y + rect.size.y);

    Vec2 intersection = { 0, 0 };
    if (i1.x <= i2.x && i1.y <= i2.y) {
        intersection.x = i1.x;
        intersection.y = i1.y;
    }

    return intersection;
}

bool rectsOverlap(Rect r1, Rect r2) {
    // Check if any corner of r2 is inside r1
    if (pointInRect(r2.position, r1) ||
        pointInRect((Vec2) { r2.position.x + r2.size.x, r2.position.y }, r1) ||
        pointInRect((Vec2) { r2.position.x, r2.position.y + r2.size.y }, r1) ||
        pointInRect((Vec2) { r2.position.x + r2.size.x, r2.position.y + r2.size.y }, r1)) {
        return true;
    }

    // Check if any corner of r1 is inside r2
    if (pointInRect(r1.position, r2) ||
        pointInRect((Vec2) { r1.position.x + r1.size.x, r1.position.y }, r2) ||
        pointInRect((Vec2) { r1.position.x, r1.position.y + r1.size.y }, r2) ||
        pointInRect((Vec2) { r1.position.x + r1.size.x, r1.position.y + r1.size.y }, r2)) {
        return true;
    }

    return false;
}

// Returns true if the point p is inside the rectangle r
bool pointInRect(Vec2 p, Rect r) {
    return p.x >= r.position.x && p.x < r.position.x + r.size.x &&
        p.y >= r.position.y && p.y < r.position.y + r.size.y;
}





// Casts a ray from the given origin in the given direction, and returns
// the point at which it intersects the first solid tile, or (0, 0) if it
// does not intersect any solid tiles before reaching its maximum length
Vec2 castRay(Ray r, Rect* tiles, int numTiles) {
    Vec2 closestIntersection = { 0, 0 };
    float closestDistance = MAX_RAY_LENGTH;

    for (int i = 0; i < numTiles; i++) {
        Vec2 intersection = intersectRect(r, tiles[i]);
        if (intersection.x != 0 || intersection.y != 0) {
            float distance = sqrtf(powf(intersection.x - r.origin.x, 2) +
                powf(intersection.y - r.origin.y, 2));
            if (distance < closestDistance) {
                closestIntersection = intersection;
                closestDistance = distance;
            }
        }
    }

    return closestIntersection;
}

bool checkRectWithinDistance(Rect rect, Ray ray, float distance) {
    // Calculate the end point of the ray
    Vec2 end_point = {
      ray.origin.x + ray.direction.x * ray.length,
      ray.origin.y + ray.direction.y * ray.length
    };

    // Check if the ray intersects the rect
    if (rect.position.x < end_point.x && end_point.x < rect.position.x + rect.size.x &&
        rect.position.y < end_point.y && end_point.y < rect.position.y + rect.size.y) {
        return true;
    }

    // Check if the rect is within the specified distance of the ray
    float rect_center_x = rect.position.x + rect.size.x / 2;
    float rect_center_y = rect.position.y + rect.size.y / 2;
    float ray_distance = (float)sqrt((rect_center_x - ray.origin.x) * (rect_center_x - ray.origin.x) +
        (rect_center_y - ray.origin.y) * (rect_center_y - ray.origin.y));
    return ray_distance <= distance;
}